#include "Inventory.h"

Inventory::Inventory()
    : positionX(positionX), positionY(positionY),
      pollenAmount(0), life(3), maxLife(3), maxEnergy(100.f), energy(100.f) 
{
    font.loadFromFile("assets/font.ttf");

    pollenTexture.loadFromFile("assets/icons/pollen.png");
    pollenIcon.setTexture(pollenTexture);
    pollenIcon.setPosition(10.f, 10.f);  // ������� ����� ����
    pollenText.setFont(font);
    pollenText.setCharacterSize(24); // ������ ������
    pollenText.setFillColor(sf::Color::White); // ���� ������
    pollenText.setPosition(pollenIcon.getPosition().x + pollenIcon.getGlobalBounds().width + 10, pollenIcon.getPosition().y); // ������� ������ ������ �� ������
    pollenText.setString(std::to_string(pollenAmount)); // ���������� ����� � ������� ����������� �������
   

    lifeIconTexture.loadFromFile("assets/icons/life.png");
    // ��������� ������ �����
    lifeIcon.setPosition(10.f, 90.f);  // ��� ������� �������
    lifeIcon.setTexture(lifeIconTexture);
    // ��������� ����� �����
    lifeBar.setSize(sf::Vector2f(200.f, 20.f));  // ����� 200 ��������, ������ 20
    lifeBar.setFillColor(sf::Color::Red);        // ������� ����� ��� ������
    lifeBar.setPosition(lifeIcon.getPosition().x + lifeIcon.getGlobalBounds().width + 10, lifeIcon.getPosition().y);  // ������� ����� ������ �� ������
    // ��������� ����� ��� ����� �����
    lifeBarBorder.setSize(sf::Vector2f(200.f, 20.f));  // ������ ����� ��, ��� � ������ ����� ������
    lifeBarBorder.setOutlineThickness(1.f);  // ������� �����
    lifeBarBorder.setOutlineColor(sf::Color::White);  // ���� �����
    lifeBarBorder.setFillColor(sf::Color::Transparent);  // ���������� ��� �����
    lifeBarBorder.setPosition(lifeBar.getPosition());  // ������� ����� ��������� � �������� �����


    energyIconTexture.loadFromFile("assets/icons/energy.png");
    // ��������� ������ �������
    energyIcon.setPosition(10.f, 150.f);  // ��� ������� �����
    energyIcon.setTexture(energyIconTexture);
    // ��������� ����� �������
    energyBar.setSize(sf::Vector2f(200.f, 20.f));  // ����� 200 ��������, ������ 20
    energyBar.setFillColor(sf::Color::Blue);       // ����� �����
    energyBar.setPosition(energyIcon.getPosition().x + energyIcon.getGlobalBounds().width + 10, energyIcon.getPosition().y);
    // ��������� ����� ��� ����� �������
    energyBarBorder.setSize(sf::Vector2f(200.f, 20.f));  // ������ ����� ��, ��� � ������ ����� �������
    energyBarBorder.setOutlineThickness(1.f);  // ������� �����
    energyBarBorder.setOutlineColor(sf::Color::White);  // ���� �����
    energyBarBorder.setFillColor(sf::Color::Transparent);  // ���������� ��� �����
    energyBarBorder.setPosition(energyBar.getPosition());  // ������� ����� ��������� � �������� �����

}


void Inventory::updateEnergyBar() {
    float energyRatio = energy / maxEnergy;  // ��������� ����������� ������� ������� � ������������
    energyBar.setSize(sf::Vector2f(200.f * energyRatio, 20.f));  // ��������� ������ �����
}

void Inventory::updateLifeBar() {
    
    float lifeRatio = life / maxLife;
    lifeBar.setSize(sf::Vector2f(200.f * lifeRatio, 20.f));  // ��������� ������ �����
}



void Inventory::draw(sf::RenderWindow& window) {
    
    window.draw(pollenIcon);
    window.draw(pollenText);

    window.draw(energyIcon);
    window.draw(energyBar);
    window.draw(energyBarBorder);

    window.draw(lifeIcon);       
    window.draw(lifeBar);         
    window.draw(lifeBarBorder);
         
    pollenText.setString(std::to_string(pollenAmount));
  
}



void Inventory::increasePollenAmount() {
    pollenAmount++; // ����������� ���������� ������
    pollenText.setString(std::to_string(pollenAmount)); // ��������� �����
}

void Inventory::decreasePollenAmount()
{
    pollenAmount--;
    pollenText.setString(std::to_string(pollenAmount)); // ��������� �����
}

void Inventory::decreaseLife(float damage) {
    if (life > 0) {
        life -= damage;
        updateLifeBar();
    }
}

void Inventory::increaseLife(float damage) {
    if (life < maxLife) {
        life += damage;;
        updateLifeBar();
    }
}

// ��������� �������
void Inventory::decreaseEnergy(float amount) {
    if (energy > 0) {
        energy -= amount;
        if (energy < 0) {
            energy = 0;
        }
    }
    //energyText.setString("Energy: " + std::to_string(energy) + "/" + std::to_string(maxEnergy));
    updateEnergyBar();
}

// ����������� ������� (��������������)
void Inventory::increaseEnergy(float amount) {
    if (energy < maxEnergy) {
        energy += amount;
        if (energy > maxEnergy) {
            energy = maxEnergy;
        }
    }
    //energyText.setString("Energy: " + std::to_string(energy) + "/" + std::to_string(maxEnergy));
    updateEnergyBar();
}
