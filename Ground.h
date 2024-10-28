#pragma once
#include <SFML/Graphics.hpp>


class Ground {
public:
    
    Ground(float positionX, float positionY, const std::string& texturePath);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    float positionX;
    float positionY;
      
private:
    sf::Texture groundTexture;        
    sf::Sprite groundSprite;    
};