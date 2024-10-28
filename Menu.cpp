#include "Menu.h"


// Конструктор
Menu::Menu(int windowWidth, int windowHeight)
     :windowWidth(windowWidth), windowHeight(windowHeight) {
    // Загрузка шрифта
    if (!font.loadFromFile("assets/font.ttf")) {
        // Обработка ошибки
    }

    // Настройка текста
    title.setFont(font);
    title.setString("Меню паузы");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, windowHeight / 4);

    // Настройка кнопок
    createButton(continueButton, "Продолжить", windowHeight / 2);
    createButton(newGameButton, "Новая игра", windowHeight / 2 + 100);
    createButton(exitButton, "Выход", windowHeight / 2 + 200);
}

// Отрисовка меню
void Menu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(continueButton);
    window.draw(newGameButton);
    window.draw(exitButton);
}

// Обработка кликов мыши
void Menu::handleClick(sf::Vector2f mousePosition, bool& isPaused, bool& isRunning) {
    if (continueButton.getGlobalBounds().contains(mousePosition)) {
        isPaused = false; // Возвращаемся в игру
    }
    else if (newGameButton.getGlobalBounds().contains(mousePosition)) {
        isRunning = false; // Запускаем новую игру
    }
    else if (exitButton.getGlobalBounds().contains(mousePosition)) {
        exit(0); // Закрываем игру
    }
}

// Создание кнопок
void Menu::createButton(sf::Text& button, const std::string& text, int yPosition) {
    button.setFont(font);
    button.setString(text);
    button.setCharacterSize(40);
    button.setFillColor(sf::Color::White);
    button.setPosition(windowWidth / 2 - button.getGlobalBounds().width / 2, yPosition);
}

