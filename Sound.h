#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>

class Sound
{
public:
    Sound();
    
    void loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);
    void setVolume(float volume);
    void stopSound(const std::string& name);
    void playBackgroundMusic(const std::string& filepath);
    bool isPlaying(const std::string& name);
    void pauseSound(const std::string& Name);

private:
    std::map<std::string, sf::SoundBuffer> soundBuffers; // Буферы для звуков
    std::map<std::string, sf::Sound> sounds;             // Звуки
    sf::SoundBuffer backgroundBuffer;  // Буфер для фоновой музыки
    sf::Sound backgroundMusic;
};

