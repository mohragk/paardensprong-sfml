#pragma once
#include <time.h>   
#include <cstdlib>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;

typedef float f32;
typedef double f64;


namespace util {
    static void toUpperCase(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }


    static sf::Font getDefaultFont() {
        sf::Font default_font;
        default_font.loadFromFile("arial.ttf");

        return default_font;
    }

    static u32 getRandomIndex(u32 length) {
        u32 number = rand() % length;
        return number;
    }

    static i32 floorMod(const i32& a, const i32& b) {
        return (a % b + b) % b;
    }
}
