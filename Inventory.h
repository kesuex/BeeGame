#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>

class Inventory
{
public:
    Inventory();
  
    void draw(sf::RenderWindow& window);

    // Методы для изменения количества пыльцы
    unsigned int getpollenAmount() const { return pollenAmount; };
    void decreasePollenAmount();
    void increasePollenAmount();

    // Методы для изменения жизни и энергии
    void decreaseLife(float damage);
    void increaseLife(float damage);
    void decreaseEnergy(float amount);
    void increaseEnergy(float amount);

    // Получение текущих значений жизни и энергии
    unsigned int getLife() const { return life; }
    unsigned int getEnergy() const { return energy; }
    unsigned int getPollenAmount() const { return pollenAmount; }

    float positionX;
    float positionY;
  
private:
    void updateLifeBar();
    void updateEnergyBar();

    sf::RectangleShape energyBar;  // Шкала энергии
    sf::RectangleShape energyBarBorder;
    sf::RectangleShape lifeBar;
    sf::RectangleShape lifeBarBorder;

    sf::Texture pollenTexture; 
    sf::Sprite pollenIcon;
    sf::Texture lifeIconTexture;
    sf::Texture energyIconTexture;

    sf::Sprite lifeIcon;
    sf::Sprite energyIcon;
    sf::Font font; 
    sf::Text pollenText;
    //sf::Text lifeText;
    //sf::Text energyText;

    unsigned int pollenAmount; 
    float life;
    float maxLife;
    float energy;
    float maxEnergy;

};


