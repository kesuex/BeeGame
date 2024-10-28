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




// ���������� ��������� �����
void Bee::Update(float deltaTime, Ground& ground, std::vector<std::unique_ptr<Flower>>& flowers, Hive* hive, Inventory& inventory) {
    if (isAlive) {

        movement(deltaTime);
        globalBoarders();
        checkGroundCollision(ground);
        shoot(inventory);
        updateEnergy(inventory, deltaTime);

        // �������� �� ������������ � ���� ������ � ������ �������
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
        tryShoot(inventory); // ���������� ��������
        canShoot = false; // ��������� �������� �� ���������� ������
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        canShoot = true; // ��������� ��������, ���� ������ �� ������
    }
}



bool Bee::flowerCollision(Flower& flower) {
    
    insectBounds = insectSprite.getGlobalBounds();
    sf::FloatRect flowerBounds = flower.getGlobalBounds();

    // ��������� ����������� ������
    if (insectBounds.intersects(flowerBounds)) {
        isOnFlower = true; // ����� �� ������
    }
    else {
        isOnFlower = false; // ����� �� �� ������
    }

    // ���������� ��������� �������� ������������
    return isOnFlower;
}


bool Bee::hiveCollision(Hive* hive) {
    insectBounds = insectSprite.getGlobalBounds();
    sf::FloatRect hiveBounds = hive->getGlobalBounds();

    // ��������� ����������� ������
    if (insectBounds.intersects(hiveBounds)) {
        isOnHive = true; // ����� �� ����
    }
    else {
        isOnHive = false; // ����� �� �� ����
    }

    // ���������� ��������� �������� ������������
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
                insectSprite.setScale(-1.f, 1.f);  // �������� ����� �����
            }
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            insectSprite.move(speed * deltaTime, 0.f);   // �������� ������
            if (insectSprite.getScale().x < 0) {
                insectSprite.setScale(1.f, 1.f);  // ���������� ������� �������
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
                insectSprite.setScale(-1.f, 1.f);  // �������� ����� �����
            }
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            insectSprite.move(speed * deltaTime, 0.f);   
            if (insectSprite.getScale().x < 0) {
                insectSprite.setScale(1.f, 1.f);  // ���������� ������� �������
            }
            
        }
       
    }
  
}

void Bee::collectPollen(Flower& flower, Inventory& inventory) {

    if (isOnFlower &&
        flower.getPollen() == 1 &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        // ���� ����� �� ������ � ������ ������� 'E', �� �������� ������
        flower.releasePollen();
        inventory.increasePollenAmount();
    }
}

void Bee::givePollen(Inventory& inventory, Hive* hive) {
    if (isOnHive &&
        inventory.getPollenAmount() > 0 &&  // ���������, ��� ������ ������ ����
        !hive->IsMaxLife() &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !isKeyPressedLastFrame) {

        inventory.decreasePollenAmount();  // ��������� ���������� ������
        hive->restoreLife();
    }

    // ��������� ��������� �������
    isKeyPressedLastFrame = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}


unsigned int Bee::getPollenCount(Inventory& inventory) const
{
    return inventory.getpollenAmount();
}


void Bee::tryShoot(Inventory& Inventory) {
    // �������� ������� ������
    if (Inventory.getpollenAmount() > 0 && canShoot) {
        shellInitialization(); // ���������� ��������
        Inventory.decreasePollenAmount();
        canShoot = false; // ��������� �������� �� ���������� ������
    }
}

// ������ ���������� ������� � �������
void Bee::updateEnergy(Inventory& inventory, float deltaTime) {
    if (isFlying) {
        inventory.decreaseEnergy(0.0005f);  // ��������� ������� ����������
        if (inventory.getEnergy() == 0)
        {
            verticalSpeed += gravity * deltaTime;
            insectSprite.move(0, verticalSpeed * deltaTime);
        }
    }
    else
    {
        inventory.increaseEnergy(0.0025f);  // ��������������� ������� ����������
    }
}

void Bee::globalBoarders()
{
    
    position = insectSprite.getPosition();
    insectBounds = insectSprite.getGlobalBounds();
    // ����������� �� �������� ������
    if (position.x - insectBounds.width / 2 < 0) {
        position.x = insectBounds.width / 2;  // ����� ����
    }
    if (position.x + insectBounds.width / 2 > windowWidth) {
        position.x = windowWidth - insectBounds.width / 2;  // ������ ����
    }
    if (position.y - insectBounds.height / 2 < 0) {
        position.y = insectBounds.height / 2;  // ������� ����
    }
    if (position.y + insectBounds.height / 2 > windowHeight) {
        position.y = windowHeight - insectBounds.height / 2;  // ������ ����
    }

    insectSprite.setPosition(position);  // ��������� ����� �������
}

void Bee::animatedWings()
{
    if (isFlying)
    {
        Insect::animatedWings();
        // ���� ���� "beeFly" �� ������, �������� ��� ���������������
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
    // ��������� �������� �� ���������� ����
    inventory.decreaseLife(damage);
    // ���������, ���� ����� ��� ������� ����� ����, ��� ����� ��������� ���� ����
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
 