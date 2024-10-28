#include "Hive.h"

Hive::Hive(int windowWidth, int windowHeight, float positionX, float positionY, const std::string& texturePath)
    : positionX(positionX), positionY(positionY), windowWidth(windowWidth), windowHeight(windowHeight), 
      life(10.f), maxlife(10.f), destroyed(false)
{
    hiveTexture.loadFromFile(texturePath);

    hiveSprite.setTexture(hiveTexture);
    hiveSprite.setPosition(positionX, positionY);
    hiveSprite.setOrigin(hiveSprite.getGlobalBounds().width / 2, hiveSprite.getGlobalBounds().height / 2);

    // Установка шкалы жизни
    hiveIndicator.setSize(sf::Vector2f(10.f, 100.f));  // Ширина 20 пикселей, высота 200 пикселей
    hiveIndicator.setFillColor(sf::Color::Yellow);       // Красная шкала для жизней

    // Установка позиции индикатора жизни (снизу слева от улья)
    hiveIndicator.setPosition(
        hiveSprite.getPosition().x - hiveIndicator.getSize().x - 80, // Смещение влево
        hiveSprite.getPosition().y + hiveSprite.getGlobalBounds().height - hiveIndicator.getSize().y -200 // На уровне низа улья
    );

    // Установка рамки для шкалы жизни
    hiveBarBorder.setSize(sf::Vector2f(10.f, 100.f));   // Размер такой же, как у полной шкалы жизней
    hiveBarBorder.setOutlineThickness(1.f);             // Толщина рамки
    hiveBarBorder.setOutlineColor(sf::Color::White);    // Цвет рамки
    hiveBarBorder.setFillColor(sf::Color::Transparent); // Прозрачный фон рамки
    hiveBarBorder.setPosition(hiveIndicator.getPosition()); // Позиция рамки совпадает с позицией шкалы
}

// Метод обновления индикатора жизни
void Hive::updateLifeBar() {
    // Высота заливки в зависимости от уровня жизни
    float fillHeight = (life / maxlife) * 100.f; // Высота заполненной части в зависимости от текущей жизни

    // Устанавливаем размер заливки
    hiveIndicator.setSize(sf::Vector2f(10.f, fillHeight));

    // Размещаем индикатор снизу вверх
    hiveIndicator.setPosition(
        hiveSprite.getPosition().x - hiveIndicator.getSize().x - 80, // Смещение влево
        hiveSprite.getPosition().y + hiveSprite.getGlobalBounds().height - fillHeight -200 // На уровне низа улья
    );
   
}


void Hive::update(float deltaTime) {
    
    if (life == 0) {

        destroyed = true;
        hiveSprite.move(0, 200.f * deltaTime); 
        //hiveIndicator.move(0, 200.f * deltaTime);
        //hiveBarBorder.move(0, 200.f * deltaTime);  // Позиция рамки совпадает с позицией шкалы
    }
    else {
        updateLifeBar(); // Обновляем бар жизни каждый кадр (если нужно)
    }
    
}

void Hive::draw(sf::RenderWindow& window) {
    window.draw(hiveSprite);
    window.draw(hiveBarBorder);  // Рисуем рамку
    window.draw(hiveIndicator);
}



void Hive::takeDamage(Inventory& inventory, float damage) {
    life -= damage;
    if (life < 0) life = 0; // Защищаем от отрицательных значений
    updateLifeBar();
}

void Hive::restoreLife() {
    life++;
    if (life > maxlife) life = maxlife; // Защищаем от значений выше максимального
    updateLifeBar();
}

bool Hive::isOffScreen(int windowWidth, int windowHeight)
{
    position = hiveSprite.getPosition();
    
    // Проверяем, выходит ли насекомое за пределы экрана
    if (position.y < 0 || position.y > windowHeight || position.x < 0 || position.x > windowWidth) {
        hiveSprite.setOrigin(hiveSprite.getGlobalBounds().width / 2, hiveSprite.getGlobalBounds().height);
        return true; // Насекомое вышло за пределы экрана
    }
    else
    {
        return false; // Насекомое на экране
    }
}


sf::FloatRect Hive::getGlobalBounds() const {
    return hiveSprite.getGlobalBounds(); // Возвращает границы спрайта земли
}

sf::Vector2f Hive::getPosition() const {
    return hiveSprite.getPosition();
}

