#pragma once


#include "util.h"
#include "CellGrid.h"
#include "TextField.h"



struct PaardensprongData {
    std::string solution;
    std::string letters[9];
    std::vector<u16> reveal_order = std::vector<u16>(9);
};



struct Game : public TextFieldListener {
    u16 window_dim_x{ 800 };
    u16 window_dim_y{ 600 };

    CellGrid cell_grid;
    TextField user_input_field;
    PaardensprongData paardensprong;
    
    std::vector<std::string> word_list;

    bool solved{ false };
    u32 game_round{ 0 };
    sf::Cursor hand_cursor, arrow_cursor;

    i32 total_score{ 0 };
    const i32 max_word_score{ 60 };
    i32 word_score{ max_word_score };
    sf::Font score_font{ util::getDefaultFont() };

    Game() {
        word_list = retrieveWordlist();
        shuffleWordlist(word_list);
        user_input_field.addListener(this);

        hand_cursor.loadFromSystem(sf::Cursor::Hand);
        arrow_cursor.loadFromSystem(sf::Cursor::Arrow);

        reset();
    }



    u32 word_index = 0;
    void reset() {
        solved = false;
        word_score = max_word_score;
        frame_count = 0;

        paardensprong = generatePaardenSprong(word_list[word_index++]);
        cell_grid = CellGrid();
        cell_grid.setupGrid(paardensprong.letters);
        game_round++;

        user_input_field.disable(false);
        user_input_field.reset();

        std::cout << paardensprong.solution << "\n";
    }

   

    void mousePressed(sf::Event::MouseButtonEvent& e) {
        if (solved) {
            bool inside = cell_grid.inBounds(e.x, e.y);
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

        fs.open("word_list.txt");

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
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine e(seed);
        std::shuffle(std::begin(list), std::end(list), e);
    }

    void handleWinState() {
        if (!solved) {
            solved = true;
            user_input_field.disable(true);
            cell_grid.reveal(0.6, paardensprong.reveal_order);
            total_score += word_score;
        }
    }

    void messageReceived(const std::string& message) override {
        std::string answer = message;
        util::toUpperCase(answer);
        if (answer == paardensprong.solution) {
            handleWinState();
        }
    }


    u32 frame_count{ 0 };
    f32 score_time_second{ 0.0f };
    void update(f32 dt) {
       
        score_time_second += dt;

        // update scores
        if (!solved) {

            if (score_time_second >= 1000.0f) {
                score_time_second = 0.0f;
                word_score--;
                if (word_score < 0) word_score = 0;

                if (word_score == 0) {
                    total_score--;
                }

            }

            if (frame_count % 60 == 0) { // 1 second elapsed
                
            }
        }
        frame_count++;

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
        user_input_field.resizeText(cell_grid.size / 6);
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
        sf::Text score_text = sf::Text(score, score_font, 48);
        score_text.setFillColor(sf::Color(sf::Color::Black));
        score_text.setPosition(48, 48);
        window.draw(score_text);

        window.display();
    }


};

