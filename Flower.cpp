#include "Flower.h"
#include <stdexcept>
#include <iostream>

Flower::Flower(float positionX, float positionY, const std::string& texturePath)
       : positionX(positionX), positionY(positionY),
         pollen(1.0f), pollenRegenRate(0.1f), isRunning(true), maxHeight(50){

    flowerTexture.loadFromFile(texturePath);
 
    flowerSprite.setTexture(flowerTexture);
    flowerSprite.setPosition(positionX, positionY);

    // Инициализация индикатора пыльцы (вертикальный)
    
    flowerIndicator.setSize(sf::Vector2f(5, maxHeight)); // Ширина 5, высота 50
    flowerIndicator.setFillColor(sf::Color::Yellow); // Цвет индикатора
    // Установка позиции индикатора пыльцы относительно позиции цветка
    flowerIndicator.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10, // Справа от цветка
        flowerSprite.getPosition().y // На уровне цветка
    );

    // Установка позиции рамки так, чтобы она совпадала с позицией индикатора
    flowerBarBorder.setPosition(flowerIndicator.getPosition());


    // Установка рамки для шкалы жизни
    flowerBarBorder.setSize(sf::Vector2f(5.f, maxHeight));   // Размер такой же, как у полной шкалы жизней
    flowerBarBorder.setOutlineThickness(1.f);             // Толщина рамки
    flowerBarBorder.setOutlineColor(sf::Color::White);    // Цвет рамки
    flowerBarBorder.setFillColor(sf::Color::Transparent); // Прозрачный фон рамки
    flowerBarBorder.setPosition(flowerIndicator.getPosition()); // Позиция рамки совпадает с позицией шкалы

    // Запуск потока восстановления пыльцы
    pollenRegenThread = std::thread(&Flower::regenPollen, this);
}

void Flower::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(pollenMutex);

    float maxHeight = 50.f;  // Фиксированная высота индикатора
    float fillHeight = maxHeight * pollen;  // Изменяемая высота заливки

    // Устанавливаем размер заливки для индикатора пыльцы
    flowerIndicator.setSize(sf::Vector2f(5, fillHeight));

    // Обновляем позицию индикатора так, чтобы он заполнялся снизу вверх
    flowerIndicator.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10,  // Справа от цветка
        flowerSprite.getPosition().y - 25 + flowerSprite.getGlobalBounds().height - fillHeight  // Заполнение от нижней части
    );

    // Обновляем позицию рамки, чтобы она совпадала с положением индикатора
    flowerBarBorder.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10,
        flowerSprite.getPosition().y - 25 + flowerSprite.getGlobalBounds().height - maxHeight
    );
}



Flower::~Flower() {
    isRunning = false;
    if (pollenRegenThread.joinable()) {
        pollenRegenThread.join();
    }
}

bool Flower::releasePollen() {
    std::lock_guard<std::mutex> lock(pollenMutex);
    if (pollen > 0) {
        pollen = 0; // Пчела собрала всю пыльцу
        return true;
    }
    return false;
}




void Flower::draw(sf::RenderWindow& window) {
    // Отрисовка цветка
    window.draw(flowerSprite);
    window.draw(flowerIndicator);
    window.draw(flowerBarBorder);
}

void Flower::regenPollen() {

    while (isRunning) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::lock_guard<std::mutex> lock(pollenMutex);
        if (pollen < 1.0f) {
            pollen += pollenRegenRate * 0.1f; // Восстановление пыльцы
            if (pollen > 1.0f) {
                pollen = 1.0f; // Ограничиваем пыльцу значением 1
            }
        }
    }
}

sf::FloatRect Flower::getGlobalBounds() const {
    return flowerSprite.getGlobalBounds(); // Возвращает границы спрайта земли
}

float Flower::getPollen() const
{
    return pollen;
}
