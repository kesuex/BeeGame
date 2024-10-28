#pragma once
#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Flower.h"
#include "Sound.h"
#include "Inventory.h"
#include "Shell.h"
#include <vector>
#include <array>
#include <algorithm>


enum class InsectType {
    BEE,
    WASP,
    
};

class Insect
{
public:
    Insect(int windowWidth, int windowHeight,float positionX, float positionY, InsectType type, Sound sound);

    InsectType type;

    virtual void takeDamage(Inventory& inventory, float damage) {}; // Для пчелы
    virtual void takeDamage() {}; // Для осы

    bool getIsAlive() const { return isAlive; }
    void draw(sf::RenderWindow& window);  // Отрисовка пчелы
    bool isOffScreen(int windowWidth, int windowHeight);

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    
    Sound sound;

protected:
    
    virtual void animatedWings();
    virtual void animatedWalk();
    virtual void die(float deltaTime);

    void checkGroundCollision(Ground& ground);
    // Проверка разделяющей оси (метод SAT)
    bool isSeparatingAxis(float minA, float maxA, float minB, float maxB);
    // Проверка столкновений с использованием SAT для прямоугольников
    bool checkSATCollision(const sf::FloatRect& rectA, const sf::FloatRect& rectB);
    void shellInitialization();
    void checkIfMoving();

    std::array<sf::Texture, 3> wingTextures;   // Массив текстур для крыльев   
    std::array<sf::Texture, 3> walkTextures;
    sf::Texture insectTexture;
    sf::Texture shellTexture; // Текстура для снарядов
    sf::Sprite insectSprite;    // Спрайт пчелы
    sf::Sprite wingSprite;
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Clock animationClock; // Часы для анимации
    std::vector<Shell> shells; // Вектор снарядов
    sf::FloatRect insectBounds;
    sf::FloatRect groundBounds;

    float positionX;
    float positionY;
    int windowWidth;  // Ширина окна
    int windowHeight;
    float animationTime;
    int wingState;  // Состояние крыльев 
    int walkState;
   
    bool canShoot; // Переменная для отслеживания возможности стрельбы
    bool isMoving;
    bool isOnGround; // Состояние на земле
    bool isGroundCollision;
    bool isFlying;
    bool isAlive;
    float fallSpeed; // Скорость падения
    float gravity;     // Гравитация
    float verticalSpeed; // Скорость падения
    float speed;
      
};







   

    