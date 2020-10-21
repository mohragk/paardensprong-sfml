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

        if (grid_index == 4) {
            grid[grid_index] = Cell();
        }
        else {
            grid[grid_index] = Cell();
            util::toUpperCase(letters[i]);
            grid[grid_index].letter = letters[i];
        }
    }
}

void CellGrid::position(f32 new_x, f32 new_y) {
    x = new_x;
    y = new_y;
}

void CellGrid::paint(sf::RenderWindow& window)
{
    u16 grid_index = 0;
    for (f32 cell_y = y; cell_y < y + dimension * cell_size; cell_y += cell_size) {
        for (f32 cell_x = x; cell_x < x + dimension * cell_size; cell_x += cell_size) {
            grid[grid_index].x = cell_x;
            grid[grid_index].y = cell_y;
            grid[grid_index].paint(window);
            grid_index++;
        }
    }
}
