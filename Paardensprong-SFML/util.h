#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>   
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>

#include <algorithm>
#include <random>
#include <chrono>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;

typedef float f32;
typedef double f64;

#define PI 3.14159265358979323846


namespace util {
    static void toUpperCase(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }


    static sf::Font getDefaultFont() {
        sf::Font default_font;
        default_font.loadFromFile("assets/arialbd.ttf");

        return default_font;
    }

    static int getRandomIndex(int length) {
        int number = rand() % length;
        return number;
    }



    static i32 floorMod(i32 a, i32 b) {
        return (a % b + b) % b;
    }

    static f32 lerp(f32 a, f32 b, f32 mu) {
        return a * (1 - mu) + b * mu;
    }

    static f64 lerp(f64 a, f64 b, f64 mu) {
        return a * (1 - mu) + b * mu;
    }


    static sf::Color colorLerp(sf::Color first, sf::Color second, f32 mu) {
        u8 r = (u8) lerp(first.r, second.r, mu);
        u8 g = (u8) lerp(first.g, second.g, mu);
        u8 b = (u8) lerp(first.b, second.b, mu);

        return sf::Color( r, g, b );
    }

    static sf::Color colorLerp(sf::Color first, sf::Color second, f64 mu) {
        u8 r = (u8) lerp(first.r, second.r, mu);
        u8 g = (u8) lerp(first.g, second.g, mu);
        u8 b = (u8) lerp(first.b, second.b, mu);

        return sf::Color(r, g, b);
    }

    static sf::Color getStandardBackgroundColor() {
        sf::Color standard_bg_color = sf::Color(200, 12, 12);
        return standard_bg_color;
    }

    static std::string getStringFromKeyCode(const sf::Keyboard::Key& k) {
        
        
        switch (k) {
            case sf::Keyboard::A: return "a";
            case sf::Keyboard::B: return "b";
            case sf::Keyboard::C: return "c";
            case sf::Keyboard::D: return "d";
            case sf::Keyboard::E: return "e";
            case sf::Keyboard::F: return "f";
            case sf::Keyboard::G: return "g";
            case sf::Keyboard::H: return "h";
            case sf::Keyboard::I: return "i";
            case sf::Keyboard::J: return "j";
            case sf::Keyboard::K: return "k";
            case sf::Keyboard::L: return "l";
            case sf::Keyboard::M: return "m";
            case sf::Keyboard::N: return "n";
            case sf::Keyboard::O: return "o";
            case sf::Keyboard::P: return "p";
            case sf::Keyboard::Q: return "q";
            case sf::Keyboard::R: return "r";
            case sf::Keyboard::S: return "s";
            case sf::Keyboard::T: return "t";
            case sf::Keyboard::U: return "u";
            case sf::Keyboard::V: return "v";
            case sf::Keyboard::W: return "w";
            case sf::Keyboard::X: return "x";
            case sf::Keyboard::Y: return "y";
            case sf::Keyboard::Z: return "z";
        }

        return "";
    }

}
