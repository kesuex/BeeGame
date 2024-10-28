#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(int windowWidth, int windowHeight);
    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f mousePosition, bool& isPaused, bool& isRunning);

private:
    sf::Font font;
    sf::Text title;
    sf::Text continueButton;
    sf::Text newGameButton;
    sf::Text exitButton;
    int windowWidth;
    int windowHeight;
    void createButton(sf::Text& button, const std::string& text, int yPosition);
};
