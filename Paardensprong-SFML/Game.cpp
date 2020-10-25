#pragma once

#include "Game.h"

Game::Game() {
   
    word_list = retrieveWordlist();
    shuffleWordlist(word_list);
    user_input_field.addListener(this);

    reset();
}

void Game::addSound(std::string url) {
    if (!sound_bank.count(url)) {
        sound_bank.insert({ url, LoadedSound() });
        if (!sound_bank[url].load(url)) {
            sound_bank.erase(url);
        }
    }
}

void Game::playSound(std::string url) {
    bool success = false;
    if (!sound_bank.count(url)) {
        sound_bank.insert({ url, LoadedSound() });
        if (sound_bank[url].load(url)) {
            success = true;
        }
        else {
            sound_bank.erase(url);
            success = false;
        }
    }
    else {
        success = true;
    }

    if (success) {
        sound_bank[url].play();
    }
}


void Game::reset() {
    solved = false;
    score_time_millis = 0.0f;
    game_round++;
    word_score = max_word_score;


    paardensprong = generatePaardenSprong(word_list[word_index++]);
    cell_grid = new CellGrid(*this);
    
    cell_grid->setupGrid(paardensprong.letters);

    user_input_field.disable(false);
    user_input_field.reset();
}


void Game::keyPressed(sf::Event::KeyEvent& e) {

    
    mouse_cursor_visible = false;
    

    if (e.alt) {
        if (e.code == sf::Keyboard::T) {
            show_avg_solve_time = !show_avg_solve_time;
        }
        return;
    }


    if (e.code == sf::Keyboard::Enter) {
        playSound("keyboard_enter_pressed.wav");
        if (solved) {
            cueue_reset = true;
            return;
        }
    }
    else {
        std::string index = std::to_string(util::getRandomIndex(4) + 1);
        playSound("keyboard_0" + index + "_pressed.wav");
    }

    user_input_field.keyPressed(e);
}

void Game::keyReleased(sf::Event::KeyEvent& e) {

    if (e.code == sf::Keyboard::Enter) {
        playSound("keyboard_enter_released.wav");

    }
    else {
        std::string num = std::to_string(util::getRandomIndex(4) + 1);
        playSound("keyboard_0" + num + "_released.wav");
    }
}



void Game::mouseMoved(sf::Event::MouseMoveEvent& e) {
    
    mouse_cursor_visible = true;
    
}

void Game::mousePressed(sf::Event::MouseButtonEvent& e) {
    if (solved) {
        bool inside = cell_grid->inBounds((f32)e.x, (f32)e.y);
        if (inside) {
            cueue_reset = true;
        }
    }
}




PaardensprongData Game::generatePaardenSprong(std::string word) {
    PaardensprongData data;

    util::toUpperCase(word);
    data.solution = word;

    u16 start = util::getRandomIndex((u32)word.size());
    i16 direction = rand() % 2 ? 1 : -1;

    u16 word_index = 0;
    for (u16 i = start; /* no bounds */; i += (direction * 3)) {
        // wrap index around
        i = util::floorMod(i, 8);
        data.letters[i] = word[word_index];
        data.reveal_order[word_index] = i;

        word_index++;
        if (word_index == 8) break;
    }
    return data;

}

std::vector<std::string> Game::retrieveWordlist() {
    std::vector<std::string> the_list = std::vector<std::string>();
    std::ifstream fs;
    std::string line;
    const std::regex reg("[a-z]+");

    fs.open("assets/word_list.txt");

    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            if (line.size() == 8) {
                if (std::regex_match(line, reg)) {
                    the_list.push_back(line);
                }
            }
        }
    }

    return the_list;
}

void Game::shuffleWordlist(std::vector<std::string>& list) {
    // obtain a time-based seed:
    u32 seed = (u32)std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(std::begin(list), std::end(list), e);
}



void Game::handleWinState() {
    if (!solved) {
        solved = true;
        total_score += word_score;
        solve_times.push_back(solve_time);


        f32 total_solve_time{ 0.0f };
        for (const f32& time : solve_times) {
            total_solve_time += time;
        }
        avg_solve_time = (total_solve_time) / game_round;
        solve_time = 0.0f;

        user_input_field.disable(true);
        cell_grid->reveal(0.6f, paardensprong.reveal_order);

        playSound("counter_bell.wav");
    }
}

void Game::actionPerformed(const std::string& message) {
    std::string answer = message;
    util::toUpperCase(answer);
    if (answer == paardensprong.solution) {
        handleWinState();
    }
}


void Game::update(f32 dt) {

    if (cueue_reset) {
        cueue_reset = false;
        reset();
    }

    // update scores
    if (!solved) {
        score_time_millis += dt;

        solve_time += dt;

        f32 one_second = 1000.0f;

        if (score_time_millis >= one_second) {
            score_time_millis = 0.0f;
            word_score--;
            if (word_score < 0) word_score = 0;

            if (word_score == 0) {
                total_score--;
                playSound("clock_tick.wav");
            }
        }

    }

    // update grid
    cell_grid->update(dt);
    cell_grid->updateWordScore(word_score);
    f32 grid_size = (f32)std::min(window_dim_y / 2, window_dim_x / 2);
    cell_grid->resize(grid_size);

    f32 grid_x = window_dim_x / 2.0f - cell_grid->size / 2.0f;
    f32 grid_y = 0.05f * window_dim_y;
    cell_grid->position(grid_x, grid_y);


    // update user input field

    f32 user_input_x = cell_grid->x + cell_grid->grid[0].border_width;
    f32 user_input_y = cell_grid->y + cell_grid->size + 48;
    user_input_field.position(user_input_x, user_input_y);
    u32 user_input_text_size = u32(cell_grid->size / 6);
    user_input_field.resizeText(user_input_text_size);
    user_input_field.update(dt);


}


void Game::render(sf::RenderWindow& window) {

    

    {
        // draw white background
        f32 w = window.getView().getSize().x;
        f32 h = window.getView().getSize().y;
        sf::RectangleShape bg = sf::RectangleShape({ w, h });
        bg.setFillColor(util::getStandardBackgroundColor());
        window.draw(bg);
    }



    cell_grid->paint(window);

    user_input_field.paint(window);


    // render total score
    std::string score = std::to_string(total_score);
    u32 score_text_size = u32(cell_grid->size / 6.0f);
    sf::Text score_text = sf::Text(score, score_font, score_text_size);
    score_text.setFillColor(sf::Color(util::getTextColor()));
    score_text.setPosition(48, 48);
    window.draw(score_text);




    // render avg solve time
    if (show_avg_solve_time) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << (avg_solve_time / 1000.0f);
        std::string mystring = ss.str();
        std::string solve_time_str = ss.str();
        sf::Text solve_time_text_shape = sf::Text(solve_time_str, score_font, score_text_size);

        f32 solve_time_x = window.getSize().x - 48.0f;
        f32 solve_time_y = 48.0f;

        sf::FloatRect bounds = solve_time_text_shape.getLocalBounds();
        f32 origin_x = bounds.left + bounds.width;
        f32 origin_y = solve_time_text_shape.getOrigin().y;

        solve_time_text_shape.setOrigin(origin_x, origin_y);
        solve_time_text_shape.setPosition({ solve_time_x, solve_time_y });


        solve_time_text_shape.setFillColor(util::getTextColor());
        window.draw(solve_time_text_shape);
    }


}