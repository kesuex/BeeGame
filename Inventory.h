#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>

class Inventory
{
public:
    Inventory();
  
    void draw(sf::RenderWindow& window);

    // ������ ��� ��������� ���������� ������
    unsigned int getpollenAmount() const { return pollenAmount; };
    void decreasePollenAmount();
    void increasePollenAmount();

    // ������ ��� ��������� ����� � �������
    void decreaseLife(float damage);
    void increaseLife(float damage);
    void decreaseEnergy(float amount);
    void increaseEnergy(float amount);

    // ��������� ������� �������� ����� � �������
    unsigned int getLife() const { return life; }
    unsigned int getEnergy() const { return energy; }
    unsigned int getPollenAmount() const { return pollenAmount; }

    float positionX;
    float positionY;
  
private:
    void updateLifeBar();
    void updateEnergyBar();

    sf::RectangleShape energyBar;  // ����� �������
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


