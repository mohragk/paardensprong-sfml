#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "util.h"


struct Cell {
    f32 x{ 0.0 }, y{ 0.0 };
    f32 size{ 96.0 };
    f32 border_width = size / 12;

    sf::Color bg_color{ sf::Color::Red };
    sf::Color text_color{ sf::Color::White };

    sf::Font font{ util::getDefaultFont() };

    std::string letter{"-"};



    void paint(sf::RenderWindow& window) {
        f32 real_size = size - border_width * 2;
        f32 real_x = x + border_width;
        f32 real_y = y + border_width;
        
        {
            sf::RectangleShape shape = sf::RectangleShape({ real_size, real_size });
            shape.setFillColor(bg_color);
            shape.setPosition(real_x, real_y);
            window.draw(shape);
        }

        {
            u16 text_size = (u16)real_size - 16;
            sf::Text text_shape = sf::Text(letter, font, text_size);
            text_shape.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = text_shape.getLocalBounds();
            text_shape.setOrigin(
                bounds.left + bounds.width/2.0f,
                bounds.top  + bounds.height/2.0f
            );

            f32 center_x = real_x + (real_size / 2.0f);
            f32 center_y = real_y + (real_size / 2.0f);

            text_shape.setFillColor(text_color);
            text_shape.setPosition(center_x, center_y);
            window.draw(text_shape);
        }
    }
};

struct CellGrid {
    static constexpr u32 GRID_COUNT = 9;
    std::vector<Cell> grid = std::vector<Cell>(GRID_COUNT);


    f32 x{ 0.0 }, y{ 0.0 };
    f32 dimension{ 3.0 };
    f32 cell_size{ 96.0 };
    f32 size{ cell_size * dimension };

    u32 world_dim_w = 800;
    u32 world_sim_h = 600;

    CellGrid(std::string letters[]) {
        setupGrid(letters);
    }

    u16 getGridIndexForLetterIndex(u16 letter_index);

    void setupGrid(std::string letters[]);

    void position(f32 new_x, f32 new_y);

    void paint(sf::RenderWindow& window);
};