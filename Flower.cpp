#include "Flower.h"
#include <stdexcept>
#include <iostream>

Flower::Flower(float positionX, float positionY, const std::string& texturePath)
       : positionX(positionX), positionY(positionY),
         pollen(1.0f), pollenRegenRate(0.1f), isRunning(true), maxHeight(50){

    flowerTexture.loadFromFile(texturePath);
 
    flowerSprite.setTexture(flowerTexture);
    flowerSprite.setPosition(positionX, positionY);

    // ������������� ���������� ������ (������������)
    
    flowerIndicator.setSize(sf::Vector2f(5, maxHeight)); // ������ 5, ������ 50
    flowerIndicator.setFillColor(sf::Color::Yellow); // ���� ����������
    // ��������� ������� ���������� ������ ������������ ������� ������
    flowerIndicator.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10, // ������ �� ������
        flowerSprite.getPosition().y // �� ������ ������
    );

    // ��������� ������� ����� ���, ����� ��� ��������� � �������� ����������
    flowerBarBorder.setPosition(flowerIndicator.getPosition());


    // ��������� ����� ��� ����� �����
    flowerBarBorder.setSize(sf::Vector2f(5.f, maxHeight));   // ������ ����� ��, ��� � ������ ����� ������
    flowerBarBorder.setOutlineThickness(1.f);             // ������� �����
    flowerBarBorder.setOutlineColor(sf::Color::White);    // ���� �����
    flowerBarBorder.setFillColor(sf::Color::Transparent); // ���������� ��� �����
    flowerBarBorder.setPosition(flowerIndicator.getPosition()); // ������� ����� ��������� � �������� �����

    // ������ ������ �������������� ������
    pollenRegenThread = std::thread(&Flower::regenPollen, this);
}

void Flower::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(pollenMutex);

    float maxHeight = 50.f;  // ������������� ������ ����������
    float fillHeight = maxHeight * pollen;  // ���������� ������ �������

    // ������������� ������ ������� ��� ���������� ������
    flowerIndicator.setSize(sf::Vector2f(5, fillHeight));

    // ��������� ������� ���������� ���, ����� �� ���������� ����� �����
    flowerIndicator.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10,  // ������ �� ������
        flowerSprite.getPosition().y - 25 + flowerSprite.getGlobalBounds().height - fillHeight  // ���������� �� ������ �����
    );

    // ��������� ������� �����, ����� ��� ��������� � ���������� ����������
    flowerBarBorder.setPosition(
        flowerSprite.getPosition().x + flowerSprite.getGlobalBounds().width + 10,
        flowerSprite.getPosition().y - 25 + flowerSprite.getGlobalBounds().height - maxHeight
    );
}



Flower::~Flower() {
    isRunning = false;
    if (pollenRegenThread.joinable()) {
        pollenRegenThread.join();
    }
}

bool Flower::releasePollen() {
    std::lock_guard<std::mutex> lock(pollenMutex);
    if (pollen > 0) {
        pollen = 0; // ����� ������� ��� ������
        return true;
    }
    return false;
}




void Flower::draw(sf::RenderWindow& window) {
    // ��������� ������
    window.draw(flowerSprite);
    window.draw(flowerIndicator);
    window.draw(flowerBarBorder);
}

void Flower::regenPollen() {

    while (isRunning) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::lock_guard<std::mutex> lock(pollenMutex);
        if (pollen < 1.0f) {
            pollen += pollenRegenRate * 0.1f; // �������������� ������
            if (pollen > 1.0f) {
                pollen = 1.0f; // ������������ ������ ��������� 1
            }
        }
    }
}

sf::FloatRect Flower::getGlobalBounds() const {
    return flowerSprite.getGlobalBounds(); // ���������� ������� ������� �����
}

float Flower::getPollen() const
{
    return pollen;
}
