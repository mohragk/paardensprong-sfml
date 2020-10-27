#include "util.h"

struct Memory {
    bool is_initialized;
    u64 storage_size;
    void* storage;
};

struct Cell {
    f32 x,y;
    f32 size;
    sf::Font* font;
    char letter;
};

struct GameState {
    static constexpr u16 max_cells{9};
    Cell cells[max_cells];
};

static void initializeCells(GameState* game_state, Memory* memory, sf::Font *font) {

    // initialize cells
    for (i16 cell_index = 0; cell_index < game_state->max_cells; cell_index++) {
        Cell cell = {};
        cell.x = 12.0f;
        cell.y = 98.0f * cell_index;
        cell.size = 96.0f;
        cell.letter = 'A';
        cell.font = font;
        game_state->cells[cell_index] = cell;
        
    }
}

static void drawCell(Cell* cell, sf::RenderWindow *window, GameState * game_state) {
    sf::RectangleShape rect = sf::RectangleShape({ cell->size, cell->size });
    rect.setPosition(cell->x, cell->y);
    rect.setFillColor(sf::Color::Magenta);
    window->draw(rect);

    sf::Text letter_shape = sf::Text(cell->letter, *cell->font, 34.0f);
    sf::FloatRect bounds = letter_shape.getLocalBounds();
    f32 origin_x = bounds.left + bounds.width / 2;
    f32 origin_y = bounds.top + bounds.height / 2;

    letter_shape.setOrigin(origin_x, origin_y);

    f32 center_x = cell->x + (cell->size / 2.0f);
    f32 center_y = cell->y + (cell->size / 2.0f);
    letter_shape.setPosition(center_x, center_y);
    letter_shape.setFillColor(sf::Color::Black);
    window->draw(letter_shape);
}


static void gameUpdateAndRender(Memory* memory, sf::RenderWindow* window, f32 dt, sf::Font *font) {
   
    // draw white background
    {
        sf::RectangleShape bg_rect = sf::RectangleShape({ (f32)window->getSize().x, (f32)window->getSize().y });
        bg_rect.setFillColor(sf::Color::White);
        window->draw(bg_rect);
    }
    
    GameState* game_state = (GameState*)memory->storage;
    u16 max_cells = 9;



    if (!memory->is_initialized) {

        memory->is_initialized = true;
       
        initializeCells(game_state, memory, font);
        
    }


    // Draw cells
    for (u16 cell_index = 0; cell_index < max_cells; cell_index++) {
        Cell* cell = &game_state->cells[cell_index];
        cell->x += 40.0f * (dt / 1000.0f);
        if (cell->x > window->getSize().x)
            cell->x = -cell->size;


        drawCell(cell, window, game_state);
    }
   
}

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

    

    sf::Clock clock;
    sf::Time prev_time;
  
    sf::Font default_font{ getDefaultFont() };

    Memory memory = {};
    memory.storage_size = Megabytes(128);
    memory.storage = VirtualAlloc(0, memory.storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);


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
           
        }

        window.clear();
        
        sf::Time now = clock.getElapsedTime();
        f32 dt = (now.asMicroseconds() - prev_time.asMicroseconds()) / 1000.0f;
        prev_time = now;
        gameUpdateAndRender(&memory, &window, dt, &default_font);

        window.display();
       
       
    }

    return 0;
}