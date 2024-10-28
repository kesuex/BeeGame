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
   
    // Проверка, что это снаряд осы и насекомое - пчела и наоборот
    if (creatorType == InsectType::WASP && insect.type == InsectType::BEE ||
        creatorType == InsectType::BEE && insect.type == InsectType::WASP) {
        // Если произошло столкновение
        if (insectBounds.intersects(shellBounds)) {
            // Если снаряд от осы, наносим урон пчеле
            if (creatorType == InsectType::WASP) {
                insect.takeDamage(inventory, damage);
            }
            // Если снаряд от пчелы, наносим урон осе
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
        // Если снаряд от осы, наносим урон пчеле
        if (creatorType == InsectType::WASP) {
            hive.takeDamage(inventory, damage);
            hasDamaged = true;
        }
        
    }
}


void Shell::update(float deltaTime, int windowWidth,  Ground& ground, Inventory& inventory, std::vector<Insect*>& insects, Hive* hive, float damage) {
   
    for (auto it = shells.begin(); it != shells.end();) {
        // Движение снаряда
        if (it->goingRight) {
            it->shellSprite.move(it->speed * deltaTime, 0); // Движение вправо
        } else {
            it->shellSprite.move(-it->speed * deltaTime, 0); // Движение влево
        }
        
        // Проверка столкновения с насекомыми
        bool hasCollided = false;
        for (Insect* insect : insects) {

            if (!it->hasDamaged && insect) {
                it->InsectCollision(inventory, *insect, damage);
                if (it->hasDamaged) {
                    hasCollided = true; // Столкновение произошло
                    break; // Выход из цикла, если урон нанесен
                }
            }
        }

        // Проверка столкновения с ульем
        if (!it->hasDamaged && hive) { // Проверка на nullptr
            it->HiveCollision(inventory, *hive, damage);
            if (it->hasDamaged) {
                hasCollided = true; // Столкновение с ульем произошло
            }      
        }


        // Проверка столкновения с землей
        if (!it->hasDamaged) { // Проверка на nullptr
            it->GroundCollision(ground);
            if (it->hasDamaged) {
                hasCollided = true; // Столкновение с ульем произошло
            }
        }

        // Удаление снаряда, если он вышел за экран или нанес урон
        if (hasCollided || it->isOffScreen(windowWidth)) {
            it = shells.erase(it); // Удаляем снаряд
        } else {
            ++it; // Переходим к следующему снаряду
        }     
    }
}


void Shell::drawAll(sf::RenderWindow& window)  {
    for (const auto& shell : shells) {
        shell.draw(window);        
    }
}

// Отрисовка снаряда
void Shell::draw(sf::RenderWindow& window) const {
    window.draw(shellSprite);
}


