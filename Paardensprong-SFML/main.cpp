
#include <SFML/OpenGL.hpp>
#include <iostream>

#include "CellGrid.h"




namespace Game {

    CellGrid cell_grid;

    static void update() {
        cell_grid.position( 120, 24 );
    }

    static void render(sf::RenderWindow &window) {
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
        
        Game::update();
        Game::render(window);
        
    }


    return 0;
}