#include "Shell.h"    
#include "Insect.h"
#include "Ground.h"  
#include "Inventory.h" 
#include "Hive.h"
#include <iostream>
#include <vector>



Shell::Shell(int windowWidth, int windowHeight, float positionX, float positionY, bool isGoingRight, Insect* creator)
    : goingRight(isGoingRight), hasDamaged(false), creatorType(creator->type), creator(creator), speed(300.0f)
{
    if (creatorType == InsectType::BEE) {
        shellSprite.setTexture(beeShellTexture);
    }
    if (creatorType == InsectType::WASP) {
        shellSprite.setTexture(waspShellTexture); 
    }
      
    shellSprite.setPosition(positionX, positionY);
                             
}


void Shell::loadTextures() {
    if (!beeShellTexture.loadFromFile("assets/shells/bee_shell.png")) {
        throw std::runtime_error("Failed to load bee shell texture");
    }
    if (!waspShellTexture.loadFromFile("assets/shells/wasp_shell.png")) {
        throw std::runtime_error("Failed to load wasp shell texture");
    }
}

std::vector<Shell> Shell::shells;
sf::Texture Shell::beeShellTexture;
sf::Texture Shell::waspShellTexture;



bool Shell::isOffScreen(int windowWidth) const {
    return shellSprite.getPosition().x > windowWidth || shellSprite.getPosition().x < 0;
}



void Shell::addShell(int windowWidth, int windowHeight, float positionX, float positionY, bool isGoingRight, Insect* creator) {
    
    Shell shell(windowWidth, windowHeight, positionX, positionY, isGoingRight, creator);
    
    if (isGoingRight) {
        shell.shellSprite.setScale(1.f, 1.f);  
    }
    else {
        shell.shellSprite.setScale(-1.f, 1.f); 
    }
    shells.push_back(shell);
}

void Shell::InsectCollision(Inventory& inventory, Insect& insect, float damage) {
    
    if (hasDamaged) return;

    sf::FloatRect shellBounds = shellSprite.getGlobalBounds();
    sf::FloatRect insectBounds = insect.getGlobalBounds();  
   
    // ��������, ��� ��� ������ ��� � ��������� - ����� � ��������
    if (creatorType == InsectType::WASP && insect.type == InsectType::BEE ||
        creatorType == InsectType::BEE && insect.type == InsectType::WASP) {
        // ���� ��������� ������������
        if (insectBounds.intersects(shellBounds)) {
            // ���� ������ �� ���, ������� ���� �����
            if (creatorType == InsectType::WASP) {
                insect.takeDamage(inventory, damage);
            }
            // ���� ������ �� �����, ������� ���� ���
            if (creatorType == InsectType::BEE) {
                insect.takeDamage(); 
            }
            hasDamaged = true;
        }    
    }
    
}

void Shell::GroundCollision(Ground& ground) {
    if (hasDamaged) return;
   
    sf::FloatRect shellBounds = shellSprite.getGlobalBounds();
    sf::FloatRect groundBounds = ground.getGlobalBounds();
    if (groundBounds.intersects(shellBounds))
    {
        hasDamaged = true;
    }
}

void Shell::HiveCollision(Inventory& inventory, Hive& hive, float damage) {

    if (hasDamaged) return;

    sf::FloatRect hiveBounds = hive.getGlobalBounds();
    sf::FloatRect shellBounds = shellSprite.getGlobalBounds();
    if (hiveBounds.intersects(shellBounds)) {
        // ���� ������ �� ���, ������� ���� �����
        if (creatorType == InsectType::WASP) {
            hive.takeDamage(inventory, damage);
            hasDamaged = true;
        }
        
    }
}


void Shell::update(float deltaTime, int windowWidth,  Ground& ground, Inventory& inventory, std::vector<Insect*>& insects, Hive* hive, float damage) {
   
    for (auto it = shells.begin(); it != shells.end();) {
        // �������� �������
        if (it->goingRight) {
            it->shellSprite.move(it->speed * deltaTime, 0); // �������� ������
        } else {
            it->shellSprite.move(-it->speed * deltaTime, 0); // �������� �����
        }
        
        // �������� ������������ � ����������
        bool hasCollided = false;
        for (Insect* insect : insects) {

            if (!it->hasDamaged && insect) {
                it->InsectCollision(inventory, *insect, damage);
                if (it->hasDamaged) {
                    hasCollided = true; // ������������ ���������
                    break; // ����� �� �����, ���� ���� �������
                }
            }
        }

        // �������� ������������ � �����
        if (!it->hasDamaged && hive) { // �������� �� nullptr
            it->HiveCollision(inventory, *hive, damage);
            if (it->hasDamaged) {
                hasCollided = true; // ������������ � ����� ���������
            }      
        }


        // �������� ������������ � ������
        if (!it->hasDamaged) { // �������� �� nullptr
            it->GroundCollision(ground);
            if (it->hasDamaged) {
                hasCollided = true; // ������������ � ����� ���������
            }
        }

        // �������� �������, ���� �� ����� �� ����� ��� ����� ����
        if (hasCollided || it->isOffScreen(windowWidth)) {
            it = shells.erase(it); // ������� ������
        } else {
            ++it; // ��������� � ���������� �������
        }     
    }
}


void Shell::drawAll(sf::RenderWindow& window)  {
    for (const auto& shell : shells) {
        shell.draw(window);        
    }
}

// ��������� �������
void Shell::draw(sf::RenderWindow& window) const {
    window.draw(shellSprite);
}


