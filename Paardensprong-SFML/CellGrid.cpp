#pragma once

#include <SFML/Graphics.hpp>
#include "CellGrid.h"

#include <algorithm>




int CellGrid::getGridIndexForLetterIndex(int letter_index)
{
    int mapping[9]{
        0, 1, 2,
        5, 8, 7,
        6, 3, 4
    };

    return mapping[letter_index];
}



void CellGrid::setupGrid(std::string letters[])
{
    for (int i = 0; i < GRID_COUNT; i++) {
        int grid_index = getGridIndexForLetterIndex(i);

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

void CellGrid::position(float new_x, float new_y) {
    x = new_x;
    y = new_y;
}

void CellGrid::paint(sf::RenderWindow& window)
{
    int grid_index = 0;
    for (float cell_y = y; cell_y < y + dimension * cell_size; cell_y += cell_size) {
        for (float cell_x = x; cell_x < x + dimension * cell_size; cell_x += cell_size) {
            grid[grid_index].x = cell_x;
            grid[grid_index].y = cell_y;
            grid[grid_index].paint(window);
            grid_index++;
        }
    }
}
