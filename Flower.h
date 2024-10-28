#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

class Flower {

public:
    Flower(float positionX, float positionY, const std::string& texturePath);
    ~Flower();

    // ���� ������ ������
    bool releasePollen(); // ���������� true, ���� ������ ���� �������
    // �������������� ������ � �������� �������
    void update(float deltaTime);
    // ��������� ������ � ����������
    void draw(sf::RenderWindow& window);
    // ������� ��� ���������� ������� ��������������
    void regenPollen();

    sf::FloatRect getGlobalBounds() const;

    float getPollen() const;
    float positionX;
    float positionY;

private:
    sf::Sprite flowerSprite;  // ������ ������
    sf::Texture flowerTexture;
    sf::RectangleShape flowerIndicator; // ��������� ������ ������
    sf::RectangleShape flowerBarBorder;
    float maxHeight;
    float pollen;             // ������� ������� ������
    float pollenRegenRate;    // �������� �������������� ������
    std::mutex pollenMutex;   // ���������� ��� ���������� �������� � ������
    std::thread pollenRegenThread; // ����� ��� �������������� ������
    bool isRunning;                // ���� ������ ������
};

