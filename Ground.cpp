#include "Ground.h"


Ground::Ground( float positionX, float positionY, const std::string& texturePath)
    : positionX(positionX), positionY(positionY)
{
   
    groundTexture.loadFromFile(texturePath); 
  
    groundSprite.setTexture(groundTexture);
    groundSprite.setPosition(positionX, positionY);
    groundSprite.setOrigin(groundSprite.getGlobalBounds().width / 2, groundSprite.getGlobalBounds().height / 2); 
}


void Ground::draw(sf::RenderWindow& window) {
    window.draw(groundSprite);
}


sf::FloatRect Ground::getGlobalBounds() const {
    return groundSprite.getGlobalBounds(); 
}