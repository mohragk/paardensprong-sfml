#pragma once

#include <SFML/Graphics.hpp>
#include "util.h"

#include "Cell.h"

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

    void resize(f32 new_size);

    void position(f32 new_x, f32 new_y);

    void update(f32 dt);

    void updateWordScore(i32 score);

    bool inBounds(f32 pos_x, f32 pos_y) {
        return  (pos_x >= x && pos_x < x + size) && (pos_y >= y && pos_y < y + size);
    }
   
    void reveal(const f32 time, std::vector<u16> &new_reveal_order);

    void paint(sf::RenderWindow& window);
};