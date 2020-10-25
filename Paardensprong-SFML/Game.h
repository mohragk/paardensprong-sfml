#pragma once


#include "util.h"

#include "TextField.h"
#include "CellGrid.h"
#include <SFML/Audio.hpp>
#include "LoadedSound.h"



struct PaardensprongData {
    std::string solution;
    std::string letters[9];
    std::vector<u16> reveal_order = std::vector<u16>(9);
};

struct Game : public TextFieldListener {
    u16 window_dim_x{ 800 };
    u16 window_dim_y{ 600 };

    
    std::unordered_map<std::string, LoadedSound> sound_bank;

    CellGrid *cell_grid;
    TextField user_input_field;
    PaardensprongData paardensprong;
    
    u32 word_index = 0;
    std::vector<std::string> word_list;

    bool solved{ false };
    u32 game_round{ 0 };
    

    bool show_avg_solve_time{ false };
    f32 solve_time{ 0.0f };
    std::vector<f32> solve_times = std::vector<f32>();
    f32 avg_solve_time{ 0.0f };


    f32 score_time_millis{ 0.0f };
    i32 total_score{ 0 };
    const i32 max_word_score{ 30 };
    i32 word_score{ max_word_score };
    sf::Font score_font{ util::getDefaultFont() };
    bool cueue_reset{ false };

    bool mouse_cursor_visible{ true };
      
    Game();
    ~Game() {
        delete cell_grid;
    }

    void addSound(std::string url);
    void playSound(std::string url);
    
    void reset();


    void keyPressed(sf::Event::KeyEvent& e);
    void keyReleased(sf::Event::KeyEvent& e);

    void mouseMoved(sf::Event::MouseMoveEvent& e);
    void mousePressed(sf::Event::MouseButtonEvent& e);

   
    PaardensprongData generatePaardenSprong(std::string word);
    
    std::vector<std::string> retrieveWordlist();

    void shuffleWordlist(std::vector<std::string>& list);


    void handleWinState();

    void actionPerformed(const std::string& message) override;
   
    void update(f32 dt);

    void beginRender(sf::RenderWindow& window) {
        window.clear();
    }
    void endRender(sf::RenderWindow& window) {
        window.display();
    }

    void render(sf::RenderWindow& window);


};


