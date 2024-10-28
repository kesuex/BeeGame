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
    float detectionX = targetContainer->detectionX;  // Радиус обнаружения
    float detectionY = targetContainer->detectionY;
    float distanceX = 0.f;
    bool isInSameYPosition = false;

    position = insectSprite.getPosition();
    auto targetPositionOpt = targetContainer->getPriorityTargetPosition(position);  // Позиция игрока (пчелы)

    if (targetPositionOpt) { // Проверяем, была ли возвращена валидная позиция

        targetPosition = *targetPositionOpt; // Извлекаем значение из std::optional
        distanceX = std::abs(position.x - targetPosition.x);

        // Проверяем, находится ли пчела на одной вертикали с осью
        isInSameYPosition = (std::abs(position.y - targetPosition.y) < detectionY); //   значение для вертикального диапазона
        // Возвращаем true, если игрок в радиусе обнаружения и на одной вертикали
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
        // Определяем положение осы относительно пчелы: слева или справа

        calculateTargetDiretion(targetContainer);

        float distance = std::hypot(direction.x, direction.y); // Расстояние до целевой позиции
        float moveSpeed = 300.f; // скорость перемещения

        if (distance > 1.f) { // Если оса не достигла целевой позиции
            // Нормализуем вектор направления и умножаем на скорость
            direction /= distance; // Нормализация
            velocity = direction * moveSpeed;
            insectSprite.move(velocity * deltaTime); // Двигаем осу

        }
        else {
            // Достигли целевой позиции
            velocity = sf::Vector2f(0.f, 0.f); // Обнуляем скорость
            isPositionSet = true; 

        }

        if (isGroundCollision) {
            velocity = sf::Vector2f(0.f, 0.f); // Обнуляем скорость
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

        // Определяем целевую позицию
        if (position.x < targetPosition.x) {
            // Если оса находится слева от пчелы
            attackPosition = sf::Vector2f(targetPosition.x - 400.f, targetPosition.y);
            insectSprite.setScale(1.f, 1.f);
        }
        else {
            // Если оса находится справа от пчелы
            attackPosition = sf::Vector2f(targetPosition.x + 400.f, targetPosition.y);
            insectSprite.setScale(-1.f, 1.f); // Отразить по горизонтали
        }


        if (attackPosition.x < waspWidth / 2) {
            attackPosition.x = waspWidth / 2; // Не выходим за левую границу

        }
        else if (attackPosition.x > windowWidth - waspWidth / 2) {
            attackPosition.x = windowWidth - waspWidth / 2; // Не выходим за правую границу

        }

        direction = attackPosition - position;
    }
}


void Wasp::shoot()
{
    if (canShoot) {
        // Логика стрельбы
        shellInitialization();
        // Устанавливаем задержку перед следующей возможной стрельбой
        canShoot = false;
        // Таймер на восстановление возможности стрельбы
        shootCooldownClock.restart();
    }
    // Восстанавливаем возможность стрельбы после определённого времени
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
    // Проверяем, что следующая позиция не выходит за границы
    bool hitLeftOrRightBorder = (nextPosition.x - insectBounds.width / 2 <= 0 || nextPosition.x + insectBounds.width / 2 >= windowWidth);
    bool hitTopOrBottomBorder = (nextPosition.y - insectBounds.height / 2 <= 0 || nextPosition.y + insectBounds.height / 2 >= windowHeight);

    if (hitLeftOrRightBorder) {direction.x = -direction.x;} // Меняем горизонтальное направление, если достигли левой или правой границы     
    if (hitTopOrBottomBorder) {direction.y = -direction.y;} // Меняем вертикальное направление, если достигли верхней или нижней границы
           
    if (direction.x > 0) { insectSprite.setScale(1.f, 1.f); }  // Отражаем пчелу вправо    
    else { insectSprite.setScale(-1.f, 1.f); }  // Отражаем пчелу влево

    if(isOnGround){ speed = 100.f; } else{ speed = 150.f; }

    insectSprite.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    
}




void Wasp::patrol(float deltaTime) {
    // Патрулирование - оса движется вперед и меняет направление при столкновении с границами
 
    if (isPatroling) {
        // Если оса движется, проверяем продолжительность движения
        if (movementClock.getElapsedTime().asSeconds() < moveDuration) {

            movement(deltaTime);   
        }
        else {
            // Движение завершилось, переходим к паузе
            isPatroling = false;
            pauseClock.restart();
            
        }
    }
    else {
        // Если оса на паузе, проверяем продолжительность паузы
        if (pauseClock.getElapsedTime().asSeconds() >= pauseDuration) {
            // Пауза закончилась, начинаем новое движение
            isPatroling = true;
            randomizeDirection(isOnGround);  // Генерируем новое случайное направление
            movementClock.restart();
        }
    }      
}









void Wasp::randomizeDirection(bool isOnGround) {
    float angle;

    if (isOnGround) {
        // Если оса на земле, выбираем угол в пределах от 0 до 180 градусов
        angle = static_cast<float>(rand() % 180);  // Угол направления от 0 до 180
    }
    else {
        // Если оса не на земле, выбираем любой угол от 0 до 360 градусов
        angle = static_cast<float>(rand() % 360);  // Угол направления от 0 до 360
    }

    float radians = angle * 3.14159f / 180.0f;  // Переводим угол в радианы
    direction.x = cos(radians);  // Горизонтальная составляющая
    direction.y = sin(radians);  // Вертикальная составляющая

    // Если оса на земле, и направление все еще немного указывает вниз (с погрешностью), корректируем
    if (isOnGround && direction.y > 0) {
        direction.y = -direction.y;  // Инвертируем вертикальную составляющую, чтобы двигаться вверх
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

