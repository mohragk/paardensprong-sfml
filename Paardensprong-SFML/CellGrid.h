#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "util.h"


struct Cell {
    float x{ 0.0 }, y{ 0.0 };
    float size{ 96.0 };
    float border_width = size / 12;

    sf::Color bg_color{ sf::Color::Red };
    sf::Color text_color{ sf::Color::White };

    sf::Font font{ util::getDefaultFont() };

    std::string letter{"-"};



    void paint(sf::RenderWindow& window) {
        float real_size = size - border_width * 2;
        float real_x = x + border_width;
        float real_y = y + border_width;
        
        {
            sf::RectangleShape shape = sf::RectangleShape({ real_size, real_size });
            shape.setFillColor(bg_color);
            shape.setPosition(real_x, real_y);
            window.draw(shape);
        }

        {
            float text_size = real_size - 16;
            sf::Text text_shape = sf::Text(letter, font, text_size);
            text_shape.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = text_shape.getLocalBounds();
            text_shape.setOrigin(
                bounds.left + bounds.width/2.0f,
                bounds.top  + bounds.height/2.0f
            );

            float center_x = real_x + (real_size / 2.0f);
            float center_y = real_y + (real_size / 2.0f);

            text_shape.setFillColor(text_color);
            text_shape.setPosition(center_x, center_y);
            window.draw(text_shape);
        }
    }
};

struct CellGrid {
    static constexpr int GRID_COUNT = 9;
    std::vector<Cell> grid = std::vector<Cell>(GRID_COUNT);


    float x{ 0.0 }, y{ 0.0 };
    float dimension{ 3.0 };
    float cell_size{ 96.0 };
    float size{ cell_size * dimension };

    CellGrid(std::string letters[]) {
        setupGrid(letters);
    }

    int getGridIndexForLetterIndex(int letter_index);

    void setupGrid(std::string letters[]);

    void position(float new_x, float new_y);

    void paint(sf::RenderWindow& window);
};