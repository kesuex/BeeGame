#include "Menu.h"


// �����������
Menu::Menu(int windowWidth, int windowHeight)
     :windowWidth(windowWidth), windowHeight(windowHeight) {
    // �������� ������
    if (!font.loadFromFile("assets/font.ttf")) {
        // ��������� ������
    }

    // ��������� ������
    title.setFont(font);
    title.setString("���� �����");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, windowHeight / 4);

    // ��������� ������
    createButton(continueButton, "����������", windowHeight / 2);
    createButton(newGameButton, "����� ����", windowHeight / 2 + 100);
    createButton(exitButton, "�����", windowHeight / 2 + 200);
}

// ��������� ����
void Menu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(continueButton);
    window.draw(newGameButton);
    window.draw(exitButton);
}

// ��������� ������ ����
void Menu::handleClick(sf::Vector2f mousePosition, bool& isPaused, bool& isRunning) {
    if (continueButton.getGlobalBounds().contains(mousePosition)) {
        isPaused = false; // ������������ � ����
    }
    else if (newGameButton.getGlobalBounds().contains(mousePosition)) {
        isRunning = false; // ��������� ����� ����
    }
    else if (exitButton.getGlobalBounds().contains(mousePosition)) {
        exit(0); // ��������� ����
    }
}

// �������� ������
void Menu::createButton(sf::Text& button, const std::string& text, int yPosition) {
    button.setFont(font);
    button.setString(text);
    button.setCharacterSize(40);
    button.setFillColor(sf::Color::White);
    button.setPosition(windowWidth / 2 - button.getGlobalBounds().width / 2, yPosition);
}

