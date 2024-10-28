#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

class Flower {

public:
    Flower(float positionX, float positionY, const std::string& texturePath);
    ~Flower();

    // Сбор пыльцы пчелой
    bool releasePollen(); // Возвращает true, если пыльца была собрана
    // Восстановление пыльцы с течением времени
    void update(float deltaTime);
    // Отрисовка цветка и индикатора
    void draw(sf::RenderWindow& window);
    // Функция для управления потоком восстановления
    void regenPollen();

    sf::FloatRect getGlobalBounds() const;

    float getPollen() const;
    float positionX;
    float positionY;

private:
    sf::Sprite flowerSprite;  // Спрайт цветка
    sf::Texture flowerTexture;
    sf::RectangleShape flowerIndicator; // Индикатор уровня пыльцы
    sf::RectangleShape flowerBarBorder;
    float maxHeight;
    float pollen;             // Текущий уровень пыльцы
    float pollenRegenRate;    // Скорость восстановления пыльцы
    std::mutex pollenMutex;   // Блокировка для управления доступом к пыльце
    std::thread pollenRegenThread; // Поток для восстановления пыльцы
    bool isRunning;                // Флаг работы потока
};

