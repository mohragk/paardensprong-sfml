
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>   

#include "Game.h"


#define eol "\n"



int main()
{
    srand((u32) time(NULL));
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    std::string window_title = "Paardensprong Game";
    u16 window_width = 1024;
    u16 window_height = 768;
    u16 prev_window_width = window_width;
    u16 prev_window_height = window_height;
    i16 prev_window_pos_x{ 0 };
    i16 prev_window_pos_y{ 0 };

    bool is_fullscreen{ false };


    sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_title, sf::Style::Titlebar | sf::Style::Close, settings );
    prev_window_pos_x = window.getPosition().x;
    prev_window_pos_y = window.getPosition().y;
    // activate the window
    window.setActive(true);
    window.setVerticalSyncEnabled(true);

    // create the game
    Game game;
    game.window = &window;

    sf::Clock clock;
    sf::Time prev_time;
  
    // main "loop"
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {

                std::cout << "window resized" << eol;
               
                // Reset view
                sf::View view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);

                window_width = window.getSize().x;
                window_height = window.getSize().y;
            }

             

            else if (event.type == sf::Event::MouseMoved) {
                sf::Cursor *cur  = game.getMouseCursorForPosition((f32)event.mouseMove.x, (f32)event.mouseMove.y);
                if (cur != nullptr) {
                    window.setMouseCursor(*cur);
                }

                game.mouseMoved(event.mouseMove);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                game.mousePressed(event.mouseButton);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return 0;
                }

                else if (event.key.code == sf::Keyboard::F11) {
                    is_fullscreen = !is_fullscreen;
                    if (is_fullscreen) {

                        prev_window_width = window.getSize().x;
                        prev_window_height = window.getSize().y;
                        prev_window_pos_x = window.getPosition().x;
                        prev_window_pos_y = window.getPosition().y;

                        window.create(sf::VideoMode::getDesktopMode(), window_title, sf::Style::None, settings);
                        window.setPosition({ 0,0 });
                       
                    }
                    else {
                        
                        window.create(sf::VideoMode(prev_window_width, prev_window_height), window_title, sf::Style::Titlebar | sf::Style::Close, settings);
                        window.setPosition(sf::Vector2i(prev_window_pos_x < 0 ? 0 : prev_window_pos_x, prev_window_pos_y < 0 ? 0 : prev_window_pos_y));
                    }
                    window_width = window.getSize().x;
                    window_height = window.getSize().y;

                }

                else {
                    game.keyPressed(event.key);
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                game.keyReleased(event.key);
            }
                
        }

        // main game update and render "loop"
        game.window_dim_x = window_width;
        game.window_dim_y = window_height;

        sf::Time now = clock.getElapsedTime();
        f32 dt = (now.asMicroseconds() - prev_time.asMicroseconds()) / 1000.0f;
        prev_time = now;
       

        game.update(dt);
        game.beginRender(window);
        game.render(window);
        game.endRender(window);
       
        
       
    }


    return 0;
}