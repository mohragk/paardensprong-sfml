
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>   

#include "CellGrid.h"


struct PaardensprongData {
    std::string solution;
    std::string letters[9];
    std::vector<int> reveal_order = std::vector<int>(9);
};



struct Game {
   
    std::string word{ "asperges" };
    PaardensprongData paardensprong = Game::generatePaardenSprong(word);
    CellGrid cell_grid = CellGrid(paardensprong.letters);
    

    void update() {
        cell_grid.position( 120, 24 );
    }

    PaardensprongData generatePaardenSprong(std::string &word) {
        PaardensprongData data;
        
        util::toUpperCase(word);
        data.solution = word;
        
        int start = util::getRandomIndex(word.size());
        int direction = rand() % 2 ? 1 : -1;

        int word_index = 0;
        for (int i = start; /* no bounds */; i += (direction * 3)) {
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
            float w = window.getView().getSize().x;
            float h = window.getView().getSize().y;
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
    srand(time(NULL));
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Paardensprong Game", sf::Style::Default, settings );
    
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
                sf::View view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);
                
            }
                
        }
        
        game.update();
        game.render(window);
        
    }


    return 0;
}