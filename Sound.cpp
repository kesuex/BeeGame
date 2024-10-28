#include "Sound.h"
#include <iostream>

Sound::Sound()
{
}

// Реализация в .cpp файле
void Sound::playBackgroundMusic(const std::string& filepath)
{
    // Загрузка фоновой музыки
    backgroundBuffer.loadFromFile(filepath);

    // Привязываем буфер к объекту backgroundMusic
    backgroundMusic.setBuffer(backgroundBuffer);
    backgroundMusic.setLoop(true); // Устанавливаем зацикливание
    backgroundMusic.play();        // Запускаем воспроизведение
}

void Sound::loadSound(const std::string& name, const std::string& filepath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Failed to load sound: " << filepath << std::endl;
        return;
    }

    soundBuffers[name] = buffer;
    sf::Sound sound;
    sound.setBuffer(soundBuffers[name]);
    sounds[name] = sound;
}

void Sound::playSound(const std::string& name)
{
    if (sounds.find(name) != sounds.end()) {
        sounds[name].setLoop(true);
        sounds[name].play();
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}




void Sound::setVolume(float volume)
{
    for (auto& pair : sounds) {
        pair.second.setVolume(volume);
    }
    backgroundMusic.setVolume(volume);
}

void Sound::stopSound(const std::string& name)
{
    if (sounds.find(name) != sounds.end()) {
        sounds[name].stop();
        
    }
}


void Sound::pauseSound(const std::string& name) {
    if (sounds[name].getStatus() == sf::Sound::Playing) {
        sounds[name].pause();
    }
}


bool Sound::isPlaying(const std::string& name)
{
    if (sounds.find(name) != sounds.end()) {
        return sounds[name].getStatus() == sf::Sound::Playing;
    }
    return false;
}

    


