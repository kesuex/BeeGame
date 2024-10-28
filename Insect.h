#pragma once
#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "Flower.h"
#include "Sound.h"
#include "Inventory.h"
#include "Shell.h"
#include <vector>
#include <array>
#include <algorithm>


enum class InsectType {
    BEE,
    WASP,
    
};

class Insect
{
public:
    Insect(int windowWidth, int windowHeight,float positionX, float positionY, InsectType type, Sound sound);

    InsectType type;

    virtual void takeDamage(Inventory& inventory, float damage) {}; // ��� �����
    virtual void takeDamage() {}; // ��� ���

    bool getIsAlive() const { return isAlive; }
    void draw(sf::RenderWindow& window);  // ��������� �����
    bool isOffScreen(int windowWidth, int windowHeight);

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    
    Sound sound;

protected:
    
    virtual void animatedWings();
    virtual void animatedWalk();
    virtual void die(float deltaTime);

    void checkGroundCollision(Ground& ground);
    // �������� ����������� ��� (����� SAT)
    bool isSeparatingAxis(float minA, float maxA, float minB, float maxB);
    // �������� ������������ � �������������� SAT ��� ���������������
    bool checkSATCollision(const sf::FloatRect& rectA, const sf::FloatRect& rectB);
    void shellInitialization();
    void checkIfMoving();

    std::array<sf::Texture, 3> wingTextures;   // ������ ������� ��� �������   
    std::array<sf::Texture, 3> walkTextures;
    sf::Texture insectTexture;
    sf::Texture shellTexture; // �������� ��� ��������
    sf::Sprite insectSprite;    // ������ �����
    sf::Sprite wingSprite;
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Clock animationClock; // ���� ��� ��������
    std::vector<Shell> shells; // ������ ��������
    sf::FloatRect insectBounds;
    sf::FloatRect groundBounds;

    float positionX;
    float positionY;
    int windowWidth;  // ������ ����
    int windowHeight;
    float animationTime;
    int wingState;  // ��������� ������� 
    int walkState;
   
    bool canShoot; // ���������� ��� ������������ ����������� ��������
    bool isMoving;
    bool isOnGround; // ��������� �� �����
    bool isGroundCollision;
    bool isFlying;
    bool isAlive;
    float fallSpeed; // �������� �������
    float gravity;     // ����������
    float verticalSpeed; // �������� �������
    float speed;
      
};







   

    