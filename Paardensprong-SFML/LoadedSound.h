#pragma once
#include <SFML/Audio.hpp>
#include "util.h"

struct LoadedSound {
    std::string name;
    sf::SoundBuffer buffer;
    sf::Sound sound;


    void load(std::string url) {
        if (buffer.loadFromFile("sound_wav/" + url)) {
            name = url;
            sound = sf::Sound(buffer);
        }
        else {
            name = "unloaded";
        }
    }

    void play() {
        sound.play();
    }
};

