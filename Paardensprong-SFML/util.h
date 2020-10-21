#pragma once
#include <time.h>   
#include <cstdlib>


static void toUpperCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}


static sf::Font getDefaultFont() {
    sf::Font default_font;
    default_font.loadFromFile("arial.ttf");

    return default_font;
}

static int getRandomIndex(int length) {
    int number = rand() % length;
    return number;
}

static int floorMod(const int& a, const int& b) {
    return (a % b + b) % b;
}