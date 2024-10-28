#include "Wasp.h"


Wasp::Wasp(int windowWidth, int windowHeight, float positionX, float positionY, Sound sound)
    : Insect(windowWidth, windowHeight, positionX, positionY, InsectType::WASP, sound),
      shootCooldownTime(1.0f), moveDuration(3.0f), pauseDuration(5.f), isPatroling(false), isBeeAlive(true),
      isHiveDestroyed (false),isPositionSet(false), startSpawn(true), isActiveFlag(false), missionCompleted(false),
      targetXonTheField(windowWidth - 300.f), targetXoutTheField(windowWidth + 300.f) {
   
    insectTexture.loadFromFile("assets/wasp/wasp.png");   

    wingTextures[0].loadFromFile("assets/wasp/fly/wasp_fly_1.png");     
    wingTextures[1].loadFromFile("assets/wasp/fly/wasp_fly_2.png");       
    wingTextures[2].loadFromFile("assets/wasp/fly/wasp_fly_3.png");

    walkTextures[0].loadFromFile("assets/wasp/walk/wasp_walk_1.png");
    walkTextures[1].loadFromFile("assets/wasp/walk/wasp_walk_2.png");
    walkTextures[2].loadFromFile("assets/wasp/walk/wasp_walk_3.png");
     
    randomizeDirection(isOnGround);
  
    insectSprite.setTexture(insectTexture);
    insectSprite.setPosition(positionX, positionY);  
    insectSprite.setOrigin(insectSprite.getGlobalBounds().width / 2, insectSprite.getGlobalBounds().height / 2);

}



void Wasp::Update(float deltaTime, Ground& ground, TargetContainer* targetContainer) {
   
     
    if (isAlive)
    {
        animatedWings();
        animatedWalk();
        checkGroundCollision(ground);

        if (startSpawn)
        {
            moveOnTheField(deltaTime);
        }

        if(!startSpawn && targetContainer) {

            patrol(deltaTime);
            if (detectPlayer(targetContainer)) {
                attack(deltaTime, targetContainer);
            }
            else if(isPositionSet) {
                isPositionSet = false;
             }
         }
        if (!targetContainer) {

            moveOutTheField(deltaTime);
        }

    }
    else
    {
        die(deltaTime);
    }
    checkIfMoving();

}

bool Wasp::detectPlayer(TargetContainer* targetContainer) {
    float detectionX = targetContainer->detectionX;  // ������ �����������
    float detectionY = targetContainer->detectionY;
    float distanceX = 0.f;
    bool isInSameYPosition = false;

    position = insectSprite.getPosition();
    auto targetPositionOpt = targetContainer->getPriorityTargetPosition(position);  // ������� ������ (�����)

    if (targetPositionOpt) { // ���������, ���� �� ���������� �������� �������

        targetPosition = *targetPositionOpt; // ��������� �������� �� std::optional
        distanceX = std::abs(position.x - targetPosition.x);

        // ���������, ��������� �� ����� �� ����� ��������� � ����
        isInSameYPosition = (std::abs(position.y - targetPosition.y) < detectionY); //   �������� ��� ������������� ���������
        // ���������� true, ���� ����� � ������� ����������� � �� ����� ���������
        bool detected = (distanceX < detectionX) && isInSameYPosition;
       

        checkedIsTargetAlive(targetContainer, detected);
        

        return detected;
    }

    return false;
}

void Wasp::checkedIsTargetAlive(TargetContainer* targetContainer, bool& detected)
{
    if (targetContainer->getBee() != nullptr) {
        bool isBeeAlive = targetContainer->bee->getIsAlive();

        if (!isBeeAlive && !hasAlreadyCheckedLiveCondition)
        {
            hasAlreadyCheckedLiveCondition = true;
            detected = false;
        }
    }

    if (targetContainer->getHive() != nullptr) {

        bool isHiveDestroyed = targetContainer->hive->getDestroyed();

        if (isHiveDestroyed && !hasAlreadyCheckedLiveCondition)
        {
            hasAlreadyCheckedLiveCondition = true;
            detected = false;
        }
    }
}


void Wasp::attack(float deltaTime, TargetContainer* targetContainer)
{
 
    isPatroling = false;

    setAttackPosition(deltaTime, targetContainer);

    if (isPositionSet)
    {
        shoot();
    }


}



bool Wasp::setAttackPosition(float deltaTime, TargetContainer* targetContainer)
{
  
       if (!isPositionSet) {
        // ���������� ��������� ��� ������������ �����: ����� ��� ������

        calculateTargetDiretion(targetContainer);

        float distance = std::hypot(direction.x, direction.y); // ���������� �� ������� �������
        float moveSpeed = 300.f; // �������� �����������

        if (distance > 1.f) { // ���� ��� �� �������� ������� �������
            // ����������� ������ ����������� � �������� �� ��������
            direction /= distance; // ������������
            velocity = direction * moveSpeed;
            insectSprite.move(velocity * deltaTime); // ������� ���

        }
        else {
            // �������� ������� �������
            velocity = sf::Vector2f(0.f, 0.f); // �������� ��������
            isPositionSet = true; 

        }

        if (isGroundCollision) {
            velocity = sf::Vector2f(0.f, 0.f); // �������� ��������
            isPositionSet = true; 

        }
    }
  
    return isPositionSet;
}


void Wasp::calculateTargetDiretion(TargetContainer* targetContainer)
{
    position = insectSprite.getPosition();
    auto targetPositionOpt = targetContainer->getPriorityTargetPosition(position); 

    float waspWidth = insectSprite.getGlobalBounds().width;

    if (targetPositionOpt) {

        // ���������� ������� �������
        if (position.x < targetPosition.x) {
            // ���� ��� ��������� ����� �� �����
            attackPosition = sf::Vector2f(targetPosition.x - 400.f, targetPosition.y);
            insectSprite.setScale(1.f, 1.f);
        }
        else {
            // ���� ��� ��������� ������ �� �����
            attackPosition = sf::Vector2f(targetPosition.x + 400.f, targetPosition.y);
            insectSprite.setScale(-1.f, 1.f); // �������� �� �����������
        }


        if (attackPosition.x < waspWidth / 2) {
            attackPosition.x = waspWidth / 2; // �� ������� �� ����� �������

        }
        else if (attackPosition.x > windowWidth - waspWidth / 2) {
            attackPosition.x = windowWidth - waspWidth / 2; // �� ������� �� ������ �������

        }

        direction = attackPosition - position;
    }
}


void Wasp::shoot()
{
    if (canShoot) {
        // ������ ��������
        shellInitialization();
        // ������������� �������� ����� ��������� ��������� ���������
        canShoot = false;
        // ������ �� �������������� ����������� ��������
        shootCooldownClock.restart();
    }
    // ��������������� ����������� �������� ����� ������������ �������
    if (shootCooldownClock.getElapsedTime().asSeconds() > shootCooldownTime) {
        canShoot = true;
    }
   


}

void Wasp::moveOnTheField(float deltaTime)
{
    speed = 150.f;

    position = insectSprite.getPosition();
    if (position.x > targetXonTheField)
    {
        insectSprite.move(-speed * deltaTime, 0);
        insectSprite.setScale(-1.f, 1.f);
    }
    else
    {
        startSpawn = false;
    }

}

void Wasp::moveOutTheField(float deltaTime)
{
    
    speed = 150.f;

    position = insectSprite.getPosition();
    if (position.x < targetXoutTheField)
    {
        insectSprite.move(speed * deltaTime, 0);
        insectSprite.setScale(1.f, 1.f);
        
    }
    missionCompleted = true;
}

void Wasp::movement(float deltaTime)
{  
    
    sf::Vector2f nextPosition = insectSprite.getPosition() + sf::Vector2f(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    insectBounds = insectSprite.getGlobalBounds();
    // ���������, ��� ��������� ������� �� ������� �� �������
    bool hitLeftOrRightBorder = (nextPosition.x - insectBounds.width / 2 <= 0 || nextPosition.x + insectBounds.width / 2 >= windowWidth);
    bool hitTopOrBottomBorder = (nextPosition.y - insectBounds.height / 2 <= 0 || nextPosition.y + insectBounds.height / 2 >= windowHeight);

    if (hitLeftOrRightBorder) {direction.x = -direction.x;} // ������ �������������� �����������, ���� �������� ����� ��� ������ �������     
    if (hitTopOrBottomBorder) {direction.y = -direction.y;} // ������ ������������ �����������, ���� �������� ������� ��� ������ �������
           
    if (direction.x > 0) { insectSprite.setScale(1.f, 1.f); }  // �������� ����� ������    
    else { insectSprite.setScale(-1.f, 1.f); }  // �������� ����� �����

    if(isOnGround){ speed = 100.f; } else{ speed = 150.f; }

    insectSprite.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    
}




void Wasp::patrol(float deltaTime) {
    // �������������� - ��� �������� ������ � ������ ����������� ��� ������������ � ���������
 
    if (isPatroling) {
        // ���� ��� ��������, ��������� ����������������� ��������
        if (movementClock.getElapsedTime().asSeconds() < moveDuration) {

            movement(deltaTime);   
        }
        else {
            // �������� �����������, ��������� � �����
            isPatroling = false;
            pauseClock.restart();
            
        }
    }
    else {
        // ���� ��� �� �����, ��������� ����������������� �����
        if (pauseClock.getElapsedTime().asSeconds() >= pauseDuration) {
            // ����� �����������, �������� ����� ��������
            isPatroling = true;
            randomizeDirection(isOnGround);  // ���������� ����� ��������� �����������
            movementClock.restart();
        }
    }      
}









void Wasp::randomizeDirection(bool isOnGround) {
    float angle;

    if (isOnGround) {
        // ���� ��� �� �����, �������� ���� � �������� �� 0 �� 180 ��������
        angle = static_cast<float>(rand() % 180);  // ���� ����������� �� 0 �� 180
    }
    else {
        // ���� ��� �� �� �����, �������� ����� ���� �� 0 �� 360 ��������
        angle = static_cast<float>(rand() % 360);  // ���� ����������� �� 0 �� 360
    }

    float radians = angle * 3.14159f / 180.0f;  // ��������� ���� � �������
    direction.x = cos(radians);  // �������������� ������������
    direction.y = sin(radians);  // ������������ ������������

    // ���� ��� �� �����, � ����������� ��� ��� ������� ��������� ���� (� ������������), ������������
    if (isOnGround && direction.y > 0) {
        direction.y = -direction.y;  // ����������� ������������ ������������, ����� ��������� �����
    }
}


void Wasp::takeDamage()
{
    isAlive = false;
}

void Wasp::animatedWings()
{
    if(isFlying)
    {
        Insect::animatedWings();
        
        if (!sound.isPlaying("waspFly")) {
            sound.playSound("waspFly");
        }
    }
    
}

void Wasp::animatedWalk()
{
    if (isOnGround && isMoving)
    {
        Insect::animatedWalk();
    }

    if (sound.isPlaying("waspFly") && isOnGround) {
        sound.stopSound("waspFly");
    }
}

void Wasp::die(float deltaTime)
{
    Insect::die(deltaTime);
    if (sound.isPlaying("waspFly")) {
        sound.stopSound("waspFly");
    }


}

void Wasp::setPosition(float x, float y)
{   
    positionX = x;
    positionY = y;
    insectSprite.setPosition(positionX, positionY);   
}

