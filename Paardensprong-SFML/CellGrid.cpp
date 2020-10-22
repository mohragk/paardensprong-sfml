#pragma once

#include <SFML/Graphics.hpp>
#include "CellGrid.h"

#include <algorithm>




u16 CellGrid::getGridIndexForLetterIndex(u16 letter_index)
{
    u16 mapping[9]{
        0, 1, 2,
        5, 8, 7,
        6, 3, 4
    };

    return mapping[letter_index];
}



void CellGrid::setupGrid(std::string letters[])
{
    for (u16 i = 0; i < GRID_COUNT; i++) {
        u16 grid_index = getGridIndexForLetterIndex(i);

        if (grid_index == MIDDLE_CELL) {
            grid[grid_index] = Cell();
            grid[grid_index].orig_color = sf::Color(sf::Color::White);
            grid[grid_index].text_color = sf::Color(sf::Color::Red);
        }
        else {
            grid[grid_index] = Cell();
            util::toUpperCase(letters[i]);
            grid[grid_index].letter = letters[i];
        }
    }
}

void CellGrid::resize(f32 new_size)
{
    size = new_size;
    cell_size = new_size / dimension;

    for (Cell& cell : grid) {
        cell.size = cell_size;
    }

}

void CellGrid::position(f32 new_x, f32 new_y) {
    x = new_x;
    y = new_y;
}

void CellGrid::update(f32 dt)
{
    u16 grid_index = 0;
    for (f32 cell_y = y; cell_y < y + dimension * cell_size; cell_y += cell_size) {
        for (f32 cell_x = x; cell_x < x + dimension * cell_size; cell_x += cell_size) {
            grid[grid_index].x = cell_x;
            grid[grid_index].y = cell_y;
            grid[grid_index].update(dt);
            grid_index++;
        }
    }

    if (should_reveal && reveal_order_index < reveal_order.size() - 1) {
        reveal_time += dt;

        if (reveal_time >= reveal_duration) {
            reveal_time = 0.0f;
            int letter_index = reveal_order[reveal_order_index++];
            int grid_index = getGridIndexForLetterIndex(letter_index);
            grid[grid_index].reveal(0.5f);

            std::string num = std::to_string(util::getRandomIndex(4) + 1);
            sound_bank->at("horse_gallop_0"+num+".wav").play();
        }
    }

}

void CellGrid::updateWordScore(i32 score)
{
    std::string score_str = std::to_string(score);
    grid[MIDDLE_CELL].letter = score_str;

}

void CellGrid::reveal(const f32 duration, std::vector<u16>& new_reveal_order)
{
    reveal_order = new_reveal_order;
    should_reveal = true;
    reveal_time = 0;
    reveal_duration = duration * 1000.0f;
}

void CellGrid::paint(sf::RenderWindow& window)
{
    for (Cell& cell : grid) {
        cell.paint(window);
    }
}
