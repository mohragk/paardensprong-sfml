#pragma once


#include "util.h"
#include "CellGrid.h"
#include "TextField.h"

#include <SFML/Audio.hpp>
#include <unordered_map>
#include "LoadedSound.h"

struct PaardensprongData {
    std::string solution;
    std::string letters[9];
    std::vector<u16> reveal_order = std::vector<u16>(9);
};




struct Game : public TextFieldListener {
    u16 window_dim_x{ 800 };
    u16 window_dim_y{ 600 };

    sf::RenderWindow* window;
    
    std::unordered_map<std::string, LoadedSound> sound_bank;

    CellGrid cell_grid;
    TextField user_input_field;
    PaardensprongData paardensprong;
    
    u32 word_index = 0;
    std::vector<std::string> word_list;

    bool solved{ false };
    u32 game_round{ 0 };
    sf::Cursor hand_cursor, arrow_cursor;

    f32 score_time_millis{ 0.0f };
    i32 total_score{ 0 };
    const i32 max_word_score{ 30 };
    i32 word_score{ max_word_score };
    sf::Font score_font{ util::getDefaultFont() };

      
    Game() {
        for (int i = 1; i <= 5; i++) {
            std::string num = std::to_string(i);
            addSound("keyboard_0" + num + "_pressed.wav");
            addSound("keyboard_0" + num + "_released.wav");
        }

        for (int i = 1; i <= 4; i++) {
            std::string num = std::to_string(i);
            addSound("horse_gallop_0" + num + ".wav");
        }
        addSound("keyboard_enter_pressed.wav");
        addSound("keyboard_enter_released.wav");
        addSound("clock_tick.wav");
        addSound("counter_bell.wav");

        word_list = retrieveWordlist();
        shuffleWordlist(word_list);
        user_input_field.addListener(this);

        hand_cursor.loadFromSystem(sf::Cursor::Hand);
        arrow_cursor.loadFromSystem(sf::Cursor::Arrow);

        reset();
    }

    void addSound(std::string url) {
        sound_bank.insert({ url, LoadedSound() });
        sound_bank[url].load(url);
    }

    
    void reset() {
        solved = false;
        score_time_millis = 0.0f;
        game_round++;
        word_score = max_word_score;
        
        
        paardensprong = generatePaardenSprong(word_list[word_index++]);
        cell_grid = CellGrid();
        cell_grid.sound_bank = &sound_bank;
        cell_grid.setupGrid(paardensprong.letters);

        user_input_field.disable(false);
        user_input_field.reset();

    }

    void keyPressed(sf::Event::KeyEvent& e) {
        
        if (window != nullptr) {
            window->setMouseCursorVisible(false);
        }

        if (e.code == sf::Keyboard::Enter) {
            sound_bank["keyboard_enter_pressed.wav"].play();
            if (solved) {
                reset();
                return;
            }
        }
        else {
            std::string index = std::to_string(util::getRandomIndex(4) + 1);
            sound_bank["keyboard_0" + index + "_pressed.wav"].play();
        }
        
        user_input_field.keyPressed(e);
    }

    void keyReleased(sf::Event::KeyEvent& e) {

        if (e.code == sf::Keyboard::Enter) {
            sound_bank["keyboard_enter_released.wav"].play();
         
        }
        else {
            std::string num = std::to_string(util::getRandomIndex(4) + 1);
            sound_bank["keyboard_0" + num + "_released.wav"].play();
        }
    }

    void mouseMoved(sf::Event::MouseMoveEvent& e) {
        if (window != nullptr) {
            window->setMouseCursorVisible(true);
            sf::Cursor *cursor = getMouseCursorForPosition((f32)e.x, (f32)e.y);
            window->setMouseCursor(*cursor);
        }
    }

    void mousePressed(sf::Event::MouseButtonEvent& e) {
        if (solved) {
            bool inside = cell_grid.inBounds( (f32)e.x, (f32)e.y);
            if (inside) {
                reset();
            }
        }
    }
   
    sf::Cursor * getMouseCursorForPosition(f32 mouse_x, f32 mouse_y) {
        bool inside = cell_grid.inBounds(mouse_x, mouse_y);
        if (inside) {
            return &hand_cursor;
        }
        else {
            return &arrow_cursor;
        }
    }
    

    PaardensprongData generatePaardenSprong(std::string word) {
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

    std::vector<std::string> retrieveWordlist() {
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

    void shuffleWordlist(std::vector<std::string>& list) {
        // obtain a time-based seed:
        u32 seed = (u32) std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine e(seed);
        std::shuffle(std::begin(list), std::end(list), e);
    }

    void handleWinState() {
        if (!solved) {
            solved = true;
            total_score += word_score;
           
            user_input_field.disable(true);
            cell_grid.reveal(0.6f, paardensprong.reveal_order);

            sound_bank["counter_bell.wav"].play();
        }
    }

    void actionPerformed(const std::string& message) override {
        std::string answer = message;
        util::toUpperCase(answer);
        if (answer == paardensprong.solution) {
            handleWinState();
        }
    }


   
    void update(f32 dt) {
       

        // update scores
        if (!solved) {
            score_time_millis += dt;

            f32 one_second = 1000.0f;

            if (score_time_millis >= one_second) {
                score_time_millis = 0.0f;
                word_score--;
                if (word_score < 0) word_score = 0;

                if (word_score == 0) {
                    total_score--;
                    sound_bank["clock_tick.wav"].play();
                }
            }

        }

        // update grid
        cell_grid.update(dt);
        cell_grid.updateWordScore(word_score);
        f32 grid_size = (f32) std::min(window_dim_y / 2, window_dim_x / 2);
        cell_grid.resize(grid_size);

        f32 grid_x = window_dim_x / 2.0f - cell_grid.size / 2.0f;
        f32 grid_y = 0.05f * window_dim_y;
        cell_grid.position(grid_x, grid_y);


        // update user input field

        f32 user_input_x = cell_grid.x + cell_grid.grid[0].border_width;
        f32 user_input_y = cell_grid.y + cell_grid.size + 48;
        user_input_field.position(user_input_x, user_input_y);
        u32 user_input_text_size = u32(cell_grid.size / 6);
        user_input_field.resizeText( user_input_text_size );
        user_input_field.update(dt);

       
    }

   

    void render(sf::RenderWindow& window) {
        window.clear();

        {
            // draw white background
            f32 w = window.getView().getSize().x;
            f32 h = window.getView().getSize().y;
            sf::RectangleShape bg = sf::RectangleShape({ w, h });
            bg.setFillColor(sf::Color(sf::Color::White));
            window.draw(bg);
        }



        cell_grid.paint(window);

        user_input_field.paint(window);


        // render total score
        std::string score = std::to_string(total_score);
        u32 score_text_size = u32(cell_grid.size / 6.0f);
        sf::Text score_text = sf::Text(score, score_font, score_text_size);
        score_text.setFillColor(sf::Color(sf::Color::Black));
        score_text.setPosition(48, 48);
        window.draw(score_text);

        window.display();
    }


};

