#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.h"


class Hive
{
public:

    Hive(int windowWidth, int windowHeight, float positionX, float positionY, const std::string& texturePath);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    float positionX;
    float positionY;
    void takeDamage(Inventory& inventory, float damage);
    sf::FloatRect getGlobalBounds() const;
    void restoreLife();
    float getLife() { return life; }

    bool IsMaxLife() { return life == maxlife; }
    bool isOffScreen(int windowWidth, int windowHeight);  
    bool getDestroyed() { return destroyed; }
private:
    bool destroyed;
    int windowWidth;  // Ширина окна
    int windowHeight;
    void updateLifeBar();
    float life;
    float maxlife;
    sf::Texture hiveTexture;
    sf::Sprite hiveSprite;
    sf::RectangleShape hiveIndicator;
    sf::RectangleShape hiveBarBorder;
    sf::Vector2f position;
};


