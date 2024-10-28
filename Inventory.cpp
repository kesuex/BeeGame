#include "Inventory.h"

Inventory::Inventory()
    : positionX(positionX), positionY(positionY),
      pollenAmount(0), life(3), maxLife(3), maxEnergy(100.f), energy(100.f) 
{
    font.loadFromFile("assets/font.ttf");

    pollenTexture.loadFromFile("assets/icons/pollen.png");
    pollenIcon.setTexture(pollenTexture);
    pollenIcon.setPosition(10.f, 10.f);  // Верхний левый угол
    pollenText.setFont(font);
    pollenText.setCharacterSize(24); // Размер шрифта
    pollenText.setFillColor(sf::Color::White); // Цвет текста
    pollenText.setPosition(pollenIcon.getPosition().x + pollenIcon.getGlobalBounds().width + 10, pollenIcon.getPosition().y); // Позиция текста справа от иконки
    pollenText.setString(std::to_string(pollenAmount)); // Установить текст с текущим количеством нектара
   

    lifeIconTexture.loadFromFile("assets/icons/life.png");
    // Установка иконки жизни
    lifeIcon.setPosition(10.f, 90.f);  // Под иконкой нектара
    lifeIcon.setTexture(lifeIconTexture);
    // Установка шкалы жизни
    lifeBar.setSize(sf::Vector2f(200.f, 20.f));  // Длина 200 пикселей, высота 20
    lifeBar.setFillColor(sf::Color::Red);        // Красная шкала для жизней
    lifeBar.setPosition(lifeIcon.getPosition().x + lifeIcon.getGlobalBounds().width + 10, lifeIcon.getPosition().y);  // Позиция шкалы справа от иконки
    // Установка рамки для шкалы жизни
    lifeBarBorder.setSize(sf::Vector2f(200.f, 20.f));  // Размер такой же, как у полной шкалы жизней
    lifeBarBorder.setOutlineThickness(1.f);  // Толщина рамки
    lifeBarBorder.setOutlineColor(sf::Color::White);  // Цвет рамки
    lifeBarBorder.setFillColor(sf::Color::Transparent);  // Прозрачный фон рамки
    lifeBarBorder.setPosition(lifeBar.getPosition());  // Позиция рамки совпадает с позицией шкалы


    energyIconTexture.loadFromFile("assets/icons/energy.png");
    // Установка иконки энергии
    energyIcon.setPosition(10.f, 150.f);  // Под иконкой жизни
    energyIcon.setTexture(energyIconTexture);
    // Установка шкалы энергии
    energyBar.setSize(sf::Vector2f(200.f, 20.f));  // Длина 200 пикселей, высота 20
    energyBar.setFillColor(sf::Color::Blue);       // Синяя шкала
    energyBar.setPosition(energyIcon.getPosition().x + energyIcon.getGlobalBounds().width + 10, energyIcon.getPosition().y);
    // Установка рамки для шкалы энергии
    energyBarBorder.setSize(sf::Vector2f(200.f, 20.f));  // Размер такой же, как у полной шкалы энергии
    energyBarBorder.setOutlineThickness(1.f);  // Толщина рамки
    energyBarBorder.setOutlineColor(sf::Color::White);  // Цвет рамки
    energyBarBorder.setFillColor(sf::Color::Transparent);  // Прозрачный фон рамки
    energyBarBorder.setPosition(energyBar.getPosition());  // Позиция рамки совпадает с позицией шкалы

}


void Inventory::updateEnergyBar() {
    float energyRatio = energy / maxEnergy;  // Вычисляем соотношение текущей энергии к максимальной
    energyBar.setSize(sf::Vector2f(200.f * energyRatio, 20.f));  // Обновляем ширину шкалы
}

void Inventory::updateLifeBar() {
    
    float lifeRatio = life / maxLife;
    lifeBar.setSize(sf::Vector2f(200.f * lifeRatio, 20.f));  // Обновляем ширину шкалы
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
    pollenAmount++; // Увеличиваем количество пыльцы
    pollenText.setString(std::to_string(pollenAmount)); // Обновляем текст
}

void Inventory::decreasePollenAmount()
{
    pollenAmount--;
    pollenText.setString(std::to_string(pollenAmount)); // Обновляем текст
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

// Уменьшаем энергию
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

// Увеличиваем энергию (восстановление)
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
