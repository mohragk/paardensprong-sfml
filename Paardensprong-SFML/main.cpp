
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>   
#include "CellGrid.h"


struct PaardensprongData {
    std::string solution;
    std::string letters[9];
    std::vector<u16> reveal_order = std::vector<u16>(9);
};



struct Game {
   
    std::string word{ "asperges" };
    PaardensprongData paardensprong = Game::generatePaardenSprong(word);
    CellGrid cell_grid = CellGrid(paardensprong.letters);
  

    u16 window_dim_x = 800;
    u16 window_dim_y = 600;

    void update() {
        f32 grid_x = window_dim_x / 2.0f - cell_grid.size / 2.0f;
        f32 grid_y = 0.05f * window_dim_y;
        cell_grid.position(grid_x, grid_y);
    }

    PaardensprongData generatePaardenSprong(std::string &word) {
        PaardensprongData data;
        
        util::toUpperCase(word);
        data.solution = word;
        
        u16 start = util::getRandomIndex((u32) word.size());
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

    void render(sf::RenderWindow &window) {
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
        
        window.display();
    }

   
};






int main()
{
    srand((u32) time(NULL));
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    u16 window_width = 800;
    u16 window_height = 600;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Paardensprong Game", sf::Style::Default, settings );
    
    // activate the window
    window.setActive(true);
    window.setFramerateLimit(60);

    Game game;

   
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
               
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                // Reset view
                sf::View view = sf::View(sf::FloatRect(0.f, 0.f, (float) event.size.width, (float) event.size.height));
                window.setView(view);

                window_width = event.size.width;
                window_height = event.size.height;

                game.window_dim_x = window_width;
                game.window_dim_y = window_height;
            }
                
        }
        game.update();
        game.render(window);
        
    }


    return 0;
}