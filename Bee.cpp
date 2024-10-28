#include "Bee.h"


Bee::Bee(int windowWidth, int windowHeight, float positionX, float positionY, Sound sound)
    : Insect(windowWidth, windowHeight, positionX, positionY, InsectType::BEE, sound),
     isOnFlower(false), isOnHive(false), isKeyPressedLastFrame(false)
{
    
    insectTexture.loadFromFile("assets/bee/bee.png");

    wingTextures[0].loadFromFile("assets/bee/fly/bee_fly_1.png");
  
    wingTextures[1].loadFromFile("assets/bee/fly/bee_fly_2.png");
    wingTextures[2].loadFromFile("assets/bee/fly/bee_fly_3.png");
   
    

    walkTextures[0].loadFromFile("assets/bee/walk/bee_walk_1.png");
    walkTextures[1].loadFromFile("assets/bee/walk/bee_walk_2.png");
    walkTextures[2].loadFromFile("assets/bee/walk/bee_walk_3.png");
        
    insectSprite.setTexture(insectTexture);
    insectSprite.setPosition(positionX, positionY); 
    insectSprite.setOrigin(insectSprite.getGlobalBounds().width / 2, insectSprite.getGlobalBounds().height / 2);

}




// Обновление положения пчелы
void Bee::Update(float deltaTime, Ground& ground, std::vector<std::unique_ptr<Flower>>& flowers, Hive* hive, Inventory& inventory) {
    if (isAlive) {

        movement(deltaTime);
        globalBoarders();
        checkGroundCollision(ground);
        shoot(inventory);
        updateEnergy(inventory, deltaTime);

        // Проверка на столкновения и сбор пыльцы с каждым цветком
        for (auto& flower : flowers) {
            if (flower) {
                flowerCollision(*flower);
                collectPollen(*flower, inventory);
            }
        }

        if (hive) {
            hiveCollision(hive);
            givePollen(inventory, hive);
        }

        animatedWings();
        animatedWalk();
        dangerSituation(inventory, windowHeight);
    }
    else {
        die(deltaTime);
    }

    checkIfMoving();
}


void Bee::shoot(Inventory& inventory)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canShoot) {
        tryShoot(inventory); // Выполнение выстрела
        canShoot = false; // Запретить стрельбу до следующего щелчка
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        canShoot = true; // Разрешить стрельбу, если кнопка не нажата
    }
}



bool Bee::flowerCollision(Flower& flower) {
    
    insectBounds = insectSprite.getGlobalBounds();
    sf::FloatRect flowerBounds = flower.getGlobalBounds();

    // Проверяем пересечение границ
    if (insectBounds.intersects(flowerBounds)) {
        isOnFlower = true; // Пчела на цветке
    }
    else {
        isOnFlower = false; // Пчела не на цветке
    }

    // Возвращаем результат проверки столкновения
    return isOnFlower;
}


bool Bee::hiveCollision(Hive* hive) {
    insectBounds = insectSprite.getGlobalBounds();
    sf::FloatRect hiveBounds = hive->getGlobalBounds();

    // Проверяем пересечение границ
    if (insectBounds.intersects(hiveBounds)) {
        isOnHive = true; // Пчела на улье
    }
    else {
        isOnHive = false; // Пчела не на улье
    }

    // Возвращаем результат проверки столкновения
    return isOnHive;
}




void Bee::movement(float deltaTime)
{
  
    if (isFlying)
    {
        speed = 350.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            insectSprite.move(0.f, (-speed + 150.f) * deltaTime); 
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            insectSprite.move(0.f, (speed - 50.f) * deltaTime);   
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            insectSprite.move(-speed * deltaTime, 0.f);  
            if (insectSprite.getScale().x > 0) {
                insectSprite.setScale(-1.f, 1.f);  // Отражаем пчелу влево
            }
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            insectSprite.move(speed * deltaTime, 0.f);   // Движение вправо
            if (insectSprite.getScale().x < 0) {
                insectSprite.setScale(1.f, 1.f);  // Возвращаем обычный масштаб
            }
            
        }
    }
    else if (isOnGround)
    {
        speed = 100.f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            insectSprite.move(0.f, -speed  * deltaTime);  
            
        }
       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            insectSprite.move(-speed * deltaTime, 0.f);  
            if (insectSprite.getScale().x > 0) {
                insectSprite.setScale(-1.f, 1.f);  // Отражаем пчелу влево
            }
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            insectSprite.move(speed * deltaTime, 0.f);   
            if (insectSprite.getScale().x < 0) {
                insectSprite.setScale(1.f, 1.f);  // Возвращаем обычный масштаб
            }
            
        }
       
    }
  
}

void Bee::collectPollen(Flower& flower, Inventory& inventory) {

    if (isOnFlower &&
        flower.getPollen() == 1 &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        // Если пчела на цветке и нажата клавиша 'E', то собираем пыльцу
        flower.releasePollen();
        inventory.increasePollenAmount();
    }
}

void Bee::givePollen(Inventory& inventory, Hive* hive) {
    if (isOnHive &&
        inventory.getPollenAmount() > 0 &&  // Проверяем, что пыльцы больше нуля
        !hive->IsMaxLife() &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !isKeyPressedLastFrame) {

        inventory.decreasePollenAmount();  // Уменьшаем количество пыльцы
        hive->restoreLife();
    }

    // Обновляем состояние клавиши
    isKeyPressedLastFrame = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}


unsigned int Bee::getPollenCount(Inventory& inventory) const
{
    return inventory.getpollenAmount();
}


void Bee::tryShoot(Inventory& Inventory) {
    // Проверка наличия пыльцы
    if (Inventory.getpollenAmount() > 0 && canShoot) {
        shellInitialization(); // Выполнение выстрела
        Inventory.decreasePollenAmount();
        canShoot = false; // Запретить стрельбу до следующего щелчка
    }
}

// Логика уменьшения энергии в воздухе
void Bee::updateEnergy(Inventory& inventory, float deltaTime) {
    if (isFlying) {
        inventory.decreaseEnergy(0.0005f);  // Уменьшаем энергию постепенно
        if (inventory.getEnergy() == 0)
        {
            verticalSpeed += gravity * deltaTime;
            insectSprite.move(0, verticalSpeed * deltaTime);
        }
    }
    else
    {
        inventory.increaseEnergy(0.0025f);  // Восстанавливаем энергию постепенно
    }
}

void Bee::globalBoarders()
{
    
    position = insectSprite.getPosition();
    insectBounds = insectSprite.getGlobalBounds();
    // Ограничение по границам экрана
    if (position.x - insectBounds.width / 2 < 0) {
        position.x = insectBounds.width / 2;  // Левый край
    }
    if (position.x + insectBounds.width / 2 > windowWidth) {
        position.x = windowWidth - insectBounds.width / 2;  // Правый край
    }
    if (position.y - insectBounds.height / 2 < 0) {
        position.y = insectBounds.height / 2;  // Верхний край
    }
    if (position.y + insectBounds.height / 2 > windowHeight) {
        position.y = windowHeight - insectBounds.height / 2;  // Нижний край
    }

    insectSprite.setPosition(position);  // Установка новой позиции
}

void Bee::animatedWings()
{
    if (isFlying)
    {
        Insect::animatedWings();
        // Если звук "beeFly" не играет, начинаем его воспроизведение
        if (!sound.isPlaying("beeFly")) {
            sound.playSound("beeFly");
        }
    }      

}

void Bee::animatedWalk()
{
    if (isOnGround && isMoving)
    {
        Insect::animatedWalk();
    }

    if (sound.isPlaying("beeFly") && isOnGround) {
        sound.stopSound("beeFly");
    }

}

void Bee::takeDamage(Inventory& inventory, float damage) {
    // Уменьшаем здоровье на полученный урон
    inventory.decreaseLife(damage);
    // Проверяем, если жизнь или энергия равны нулю, или пчела находится ниже нуля
    if (inventory.getLife() <= 0) {
        isAlive = false;
    }
}



void Bee::dangerSituation(Inventory& inventory, int windowHeight)
{
    if ((inventory.getEnergy() <= 0) && (this->getGlobalBounds().top + this->getGlobalBounds().height >= windowHeight)) {

        isAlive = false;
    }
}

void Bee::die(float deltaTime)
{
    Insect::die(deltaTime);
    if (sound.isPlaying("beeFly")) {
        sound.stopSound("beeFly");
    }

}
 