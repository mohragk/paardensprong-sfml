
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>   

#include "Game.h"




int main()
{
    srand((u32) time(NULL));
    sf::ContextSettings context_settings;
    context_settings.depthBits = { 24 };
    context_settings.stencilBits= { 8 };
    context_settings.antialiasingLevel= { 0 };
    context_settings.majorVersion = { 4 };
    context_settings.minorVersion= { 0 };
   
    std::string window_title{ "Paardensprong Game" };
    u16 window_width{ 1024 };
    u16 window_height{ 768 };
    u16 prev_window_width{ window_width };
    u16 prev_window_height{ window_height };
   
    bool is_fullscreen{ false };



    sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_title, sf::Style::Titlebar | sf::Style::Close, context_settings);
    window.setActive(true);
    window.setVerticalSyncEnabled(true);
    i16 prev_window_pos_x = window.getPosition().x;
    i16 prev_window_pos_y = window.getPosition().y;

    // create the game
    Game *game = new Game();

    

    sf::Clock clock;
    sf::Time prev_time;
  
    sf::Cursor hand_cursor, arrow_cursor;
    hand_cursor.loadFromSystem(sf::Cursor::Hand);
    arrow_cursor.loadFromSystem(sf::Cursor::Arrow);

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

                // Reset view
                sf::View view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                window.setView(view);

                window_width = window.getSize().x;
                window_height = window.getSize().y;
            }
            else if (event.type == sf::Event::GainedFocus)
                game->user_input_field.in_focus = true;
            else if (event.type == sf::Event::LostFocus)
                game->user_input_field.in_focus = false;
            else if (event.type == sf::Event::MouseMoved) {
                bool inside_cell = game->cell_grid->inBounds((f32)event.mouseMove.x, (f32)event.mouseMove.y);
                if (inside_cell) {
                    window.setMouseCursor(hand_cursor);
                }
                else {
                    window.setMouseCursor(arrow_cursor);
                }

                game->mouseMoved(event.mouseMove);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                game->mousePressed(event.mouseButton);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    delete game;
                    return 0;
                }

                else if (event.key.code == sf::Keyboard::F11) {
                    is_fullscreen = !is_fullscreen;
                    if (is_fullscreen) {

                        prev_window_width = window.getSize().x;
                        prev_window_height = window.getSize().y;
                        prev_window_pos_x = window.getPosition().x;
                        prev_window_pos_y = window.getPosition().y;

                        window.create(sf::VideoMode::getDesktopMode(), window_title, sf::Style::None, context_settings);
                        window.setPosition({ 0,0 });
                       
                    }
                    else {
                        
                        window.create(sf::VideoMode(prev_window_width, prev_window_height), window_title, sf::Style::Titlebar | sf::Style::Close, context_settings);
                        window.setPosition(sf::Vector2i(prev_window_pos_x < 0 ? 0 : prev_window_pos_x, prev_window_pos_y < 0 ? 0 : prev_window_pos_y));
                    }
                    window_width = window.getSize().x;
                    window_height = window.getSize().y;

                }

                else {
                    game->keyPressed(event.key);
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                game->keyReleased(event.key);
            }
                
        }

        window.setMouseCursorVisible(game->mouse_cursor_visible);

        // main game update and render "loop"
        game->window_dim_x = window_width;
        game->window_dim_y = window_height;

        sf::Time now = clock.getElapsedTime();
        f32 dt = (now.asMicroseconds() - prev_time.asMicroseconds()) / 1000.0f;
        prev_time = now;
       

        game->update(dt);
        game->beginRender(window);
        game->render(window);
        game->endRender(window);
       
        
       
    }

    delete game;
    return 0;
}