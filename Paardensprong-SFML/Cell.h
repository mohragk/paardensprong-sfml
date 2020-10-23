#pragma once

#include <SFML/Graphics.hpp>
#include "util.h"

struct Cell {
    f32 x{ -10000.0 }, y{ -10000.0 };
    f32 size{ 96.0 };
    f32 border_width = size / 12;

    sf::Color orig_color{ util::getCellOrigColor() };
    sf::Color reveal_color{ util::getCellRevealColor() };
    sf::Color text_color{ util::getCellTextColor() };
    sf::Color text_reveal_color{ util::getCellTextRevealColor() };

    f32 time_millis{ 0.0f };
    f32 reveal_duration{ 500.0f };
    f32 color_mu{ 0.0f };

    bool should_reveal{ false };

    sf::Font font{ util::getDefaultFont() };

    std::string letter{ "-" };

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

        // draw background
        {
            sf::Color bg_color = util::colorLerp(orig_color, reveal_color, color_mu);
            sf::RectangleShape shape = sf::RectangleShape({ real_size, real_size });
            shape.setFillColor(bg_color);
            shape.setPosition(real_x, real_y);
            window.draw(shape);
        }

        // draw leter
        {
            u16 text_size = u16(real_size - (real_size / 3.0f));
            sf::Text text_shape = sf::Text(letter, font, text_size);
            text_shape.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = text_shape.getLocalBounds();
            f32 origin_x = bounds.left + bounds.width / 2;
            f32 origin_y = bounds.top + bounds.height / 2;

            text_shape.setOrigin(origin_x, origin_y);

            f32 center_x = real_x + (real_size / 2.0f);
            f32 center_y = real_y + (real_size / 2.0f);

            sf::Color letter_color = util::colorLerp(text_color, text_reveal_color , color_mu);

            text_shape.setFillColor(letter_color);
            text_shape.setPosition(center_x, center_y);
            window.draw(text_shape);
        }
    }
};
