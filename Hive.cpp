#include "Hive.h"

Hive::Hive(int windowWidth, int windowHeight, float positionX, float positionY, const std::string& texturePath)
    : positionX(positionX), positionY(positionY), windowWidth(windowWidth), windowHeight(windowHeight), 
      life(10.f), maxlife(10.f), destroyed(false)
{
    hiveTexture.loadFromFile(texturePath);

    hiveSprite.setTexture(hiveTexture);
    hiveSprite.setPosition(positionX, positionY);
    hiveSprite.setOrigin(hiveSprite.getGlobalBounds().width / 2, hiveSprite.getGlobalBounds().height / 2);

    // ��������� ����� �����
    hiveIndicator.setSize(sf::Vector2f(10.f, 100.f));  // ������ 20 ��������, ������ 200 ��������
    hiveIndicator.setFillColor(sf::Color::Yellow);       // ������� ����� ��� ������

    // ��������� ������� ���������� ����� (����� ����� �� ����)
    hiveIndicator.setPosition(
        hiveSprite.getPosition().x - hiveIndicator.getSize().x - 80, // �������� �����
        hiveSprite.getPosition().y + hiveSprite.getGlobalBounds().height - hiveIndicator.getSize().y -200 // �� ������ ���� ����
    );

    // ��������� ����� ��� ����� �����
    hiveBarBorder.setSize(sf::Vector2f(10.f, 100.f));   // ������ ����� ��, ��� � ������ ����� ������
    hiveBarBorder.setOutlineThickness(1.f);             // ������� �����
    hiveBarBorder.setOutlineColor(sf::Color::White);    // ���� �����
    hiveBarBorder.setFillColor(sf::Color::Transparent); // ���������� ��� �����
    hiveBarBorder.setPosition(hiveIndicator.getPosition()); // ������� ����� ��������� � �������� �����
}

// ����� ���������� ���������� �����
void Hive::updateLifeBar() {
    // ������ ������� � ����������� �� ������ �����
    float fillHeight = (life / maxlife) * 100.f; // ������ ����������� ����� � ����������� �� ������� �����

    // ������������� ������ �������
    hiveIndicator.setSize(sf::Vector2f(10.f, fillHeight));

    // ��������� ��������� ����� �����
    hiveIndicator.setPosition(
        hiveSprite.getPosition().x - hiveIndicator.getSize().x - 80, // �������� �����
        hiveSprite.getPosition().y + hiveSprite.getGlobalBounds().height - fillHeight -200 // �� ������ ���� ����
    );
   
}


void Hive::update(float deltaTime) {
    
    if (life == 0) {

        destroyed = true;
        hiveSprite.move(0, 200.f * deltaTime); 
        //hiveIndicator.move(0, 200.f * deltaTime);
        //hiveBarBorder.move(0, 200.f * deltaTime);  // ������� ����� ��������� � �������� �����
    }
    else {
        updateLifeBar(); // ��������� ��� ����� ������ ���� (���� �����)
    }
    
}

void Hive::draw(sf::RenderWindow& window) {
    window.draw(hiveSprite);
    window.draw(hiveBarBorder);  // ������ �����
    window.draw(hiveIndicator);
}



void Hive::takeDamage(Inventory& inventory, float damage) {
    life -= damage;
    if (life < 0) life = 0; // �������� �� ������������� ��������
    updateLifeBar();
}

void Hive::restoreLife() {
    life++;
    if (life > maxlife) life = maxlife; // �������� �� �������� ���� �������������
    updateLifeBar();
}

bool Hive::isOffScreen(int windowWidth, int windowHeight)
{
    position = hiveSprite.getPosition();
    
    // ���������, ������� �� ��������� �� ������� ������
    if (position.y < 0 || position.y > windowHeight || position.x < 0 || position.x > windowWidth) {
        hiveSprite.setOrigin(hiveSprite.getGlobalBounds().width / 2, hiveSprite.getGlobalBounds().height);
        return true; // ��������� ����� �� ������� ������
    }
    else
    {
        return false; // ��������� �� ������
    }
}


sf::FloatRect Hive::getGlobalBounds() const {
    return hiveSprite.getGlobalBounds(); // ���������� ������� ������� �����
}

sf::Vector2f Hive::getPosition() const {
    return hiveSprite.getPosition();
}

