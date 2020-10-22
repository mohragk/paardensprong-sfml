#pragma once

#include <SFML/Graphics.hpp>
#include "util.h"


struct Cell {
    f32 x{ 0.0 }, y{ 0.0 };
    f32 size{ 96.0 };
    f32 border_width = size / 12;

    sf::Color orig_color{ sf::Color::Red };
    sf::Color reveal_color{ sf::Color::Black };
    sf::Color text_color{ sf::Color::White };

    f32 time_millis{ 0.0f };
    f32 reveal_duration{ 500.0f };
    f32 color_mu{ 0.0f };

    bool should_reveal{ false };
    
    sf::Font font{ util::getDefaultFont() };

    std::string letter{"-"};

    void reveal(f32 duration) {
        reveal_duration = duration * 1000.0f;
        should_reveal = true;
    }


    void update(f32 dt) {

        if (should_reveal) {
            time_millis += dt;
            color_mu = time_millis / reveal_duration;

            if (color_mu > 1.0) {
                color_mu = 1.0;
                should_reveal = false;
            }
        }

    }

    void paint(sf::RenderWindow& window) {
        f32 real_size = size - border_width * 2;
        f32 real_x = x + border_width;
        f32 real_y = y + border_width;
        
        {
            sf::Color bg_color = util::colorLerp(orig_color, reveal_color, color_mu);
            sf::RectangleShape shape = sf::RectangleShape({ real_size, real_size });
            shape.setFillColor(bg_color);
            shape.setPosition(real_x, real_y);
            window.draw(shape);
        }

        {
            u16 text_size = (u16)real_size - (real_size / 3);
            sf::Text text_shape = sf::Text(letter, font, text_size);
            text_shape.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = text_shape.getLocalBounds();
            f32 origin_x = bounds.left + bounds.width / 2;
            f32 origin_y = bounds.top + bounds.height / 2;

            text_shape.setOrigin( origin_x, origin_y );

            f32 center_x = real_x + (real_size / 2.0f);
            f32 center_y = real_y + (real_size / 2.0f);

            text_shape.setFillColor(text_color);
            text_shape.setPosition(center_x, center_y);
            window.draw(text_shape);
        }
    }
};

struct CellGrid {
    static constexpr u32 GRID_COUNT{ 9 };
    std::vector<Cell> grid = std::vector<Cell>(GRID_COUNT);

   

    f32 x{ 0.0 }, y{ 0.0 };
    f32 dimension{ 3.0 };
    f32 cell_size{ 96.0 };
    f32 size{ cell_size * dimension };

    u32 world_dim_w{ 800 };
    u32 world_sim_h{ 600 };

    std::vector<u16> reveal_order = std::vector<u16>();
    bool should_reveal{ false };
    f32 reveal_time{ 0.0f };
    f32 reveal_duration{ 0.0f };
    u32 reveal_order_index{ 0 };

    u16 getGridIndexForLetterIndex(u16 letter_index);

    void setupGrid(std::string letters[]);

    void resize(float new_size);

    void position(f32 new_x, f32 new_y);

    void update(f32 dt);

    void updateWordScore(i32 score);

    bool inBounds(f32 pos_x, f32 pos_y) {
        return  (pos_x >= x && pos_x < x + size) && (pos_y >= y && pos_y < y + size);
    }
   
    void reveal(const f32 time, std::vector<u16> &new_reveal_order);

    void paint(sf::RenderWindow& window);
};