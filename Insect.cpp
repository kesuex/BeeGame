#include "Insect.h"

Insect::Insect(int windowWidth, int windowHeight, float positionX, float positionY, InsectType type, Sound sound)
    : windowWidth(windowWidth), windowHeight(windowHeight), positionX(positionX), positionY(positionY), type(type), sound(sound),
      wingState(0),walkState(0), animationTime(0.1f), speed(0), isMoving(false), isOnGround(false),isAlive(true), isGroundCollision(false),
      isFlying(false),canShoot(false), gravity(200.0f), verticalSpeed(0.0f), fallSpeed(200.0f) {}

      
void Insect:: checkIfMoving() {
    // Получаем текущую позицию
    sf::Vector2f currentPosition = insectSprite.getPosition();

    // Если текущая позиция отличается от предыдущей
    if (currentPosition != previousPosition) {
        isMoving = true;  // Насекомое двигается
    }
    else {
        isMoving = false; // Насекомое остановилось
    }

    // Обновляем предыдущую позицию для следующей проверки
    previousPosition = currentPosition;
}


void Insect::checkGroundCollision(Ground& ground)
{
    // Получаем границы пчелы, учитывая смещение точки origin к центру
    insectBounds = insectSprite.getGlobalBounds();
    groundBounds = ground.getGlobalBounds();

    // Корректируем beeBounds, так как точка origin установлена в центр текстуры
    insectBounds.left = insectSprite.getPosition().x - insectBounds.width / 2;
    insectBounds.top = insectSprite.getPosition().y - insectBounds.height / 2;

    // Если произошло столкновение
    if (checkSATCollision(insectBounds, groundBounds)) {
        // Проверка столкновения сверху
        if (insectBounds.top + insectBounds.height <= groundBounds.top + 1) {  // Поправка на погрешность
            insectSprite.setPosition(insectSprite.getPosition().x, groundBounds.top - insectBounds.height / 2);
            isOnGround = true;
            isFlying = false;
        }

        // Проверка столкновения снизу
        else if (insectBounds.top >= groundBounds.top + groundBounds.height - 1) {  // Поправка на погрешность
            insectSprite.setPosition(insectSprite.getPosition().x, groundBounds.top + groundBounds.height + insectBounds.height / 2);          
            isFlying = true;
        }


        // Проверка столкновения слева
        else if (insectBounds.left + insectBounds.width >= groundBounds.left && insectBounds.left <= groundBounds.left) {
            insectSprite.setPosition(groundBounds.left - insectBounds.width / 2, insectSprite.getPosition().y);            
            isFlying = true;
        }

        // Проверка столкновения справа
        else if (insectBounds.left <= groundBounds.left + groundBounds.width &&
            insectBounds.left + insectBounds.width >= groundBounds.left + groundBounds.width) {
            insectSprite.setPosition(groundBounds.left + groundBounds.width + insectBounds.width / 2, insectSprite.getPosition().y);           
            isFlying = true;
        }
       
        isGroundCollision = true;
    }
    else {
        isFlying = true;
        isOnGround = false;
        isGroundCollision = false;
       
    }


    if (isOnGround)
    {
        // Определяем границы земли
        float groundLeft = groundBounds.left;
        float groundRight = groundBounds.left + groundBounds.width;

        // Получаем ширину пчелы
        float beeHalfWidth = insectBounds.width / 2;

        // Проверяем, свисает ли левая сторона пчелы за пределы земли на половину ее ширины
        if (insectBounds.left < groundLeft - beeHalfWidth) {
            // Левая сторона пчелы свисает
            isOnGround = false;  // Прекращаем считать, что на земле
            isFlying = true;     // Пчела должна летать
        }
        // Проверяем, свисает ли правая сторона пчелы за пределы земли на половину ее ширины
        else if (insectBounds.left + insectBounds.width > groundRight + beeHalfWidth) {
            // Правая сторона пчелы свисает
            isOnGround = false;  // Прекращаем считать, что на земле
            isFlying = true;     // Пчела должна летать
        }
    }

    
}

bool Insect::isSeparatingAxis(float minA, float maxA, float minB, float maxB) {
    return (maxA < minB || maxB < minA);
}

// Реализация метода проверки столкновений с использованием SAT
bool Insect::checkSATCollision(const sf::FloatRect& rectA, const sf::FloatRect& rectB) {
    // Проверка по оси X
    if (isSeparatingAxis(rectA.left, rectA.left + rectA.width, rectB.left, rectB.left + rectB.width)) {
        return false; // Нет столкновения по оси X
    }

    // Проверка по оси Y
    if (isSeparatingAxis(rectA.top, rectA.top + rectA.height, rectB.top, rectB.top + rectB.height)) {
        return false; // Нет столкновения по оси Y
    }

    // Нет разделяющей оси, значит столкновение произошло
    return true;
}

void Insect::animatedWings()
{
    if (animationClock.getElapsedTime().asSeconds() > animationTime) {
        wingState = (wingState + 1) % wingTextures.size();  // Циклическая смена состояний
        insectSprite.setTexture(wingTextures[wingState]);      // Установка новой текстуры
        animationClock.restart();                          
    }

}

void Insect::animatedWalk()
{
    if (animationClock.getElapsedTime().asSeconds() > animationTime) {
        walkState = (walkState + 1) % walkTextures.size();  // Циклическая смена состояний
        insectSprite.setTexture(walkTextures[walkState]);      // Установка новой текстуры
        animationClock.restart();
    }

}


void Insect::die(float deltaTime)
{
    insectSprite.move(0, fallSpeed * deltaTime);
    
}



// Отрисовка пчелы на экране
void Insect::draw(sf::RenderWindow& window) {
    wingSprite.setTexture(wingTextures[wingState]);
    wingSprite.setPosition(insectSprite.getPosition());  // Устанавливаем позицию на ту же, что и у тела
    window.draw(insectSprite);   // Отрисовка тела пчелы
    //window.draw(wingSprite);   // Отрисовка крыльев
}



sf::Vector2f Insect::getPosition() const
{ 
   return insectSprite.getPosition();  
}



void Insect::shellInitialization() {

    // Определение направления: если пчела смотрит влево, передаем false
    insectBounds = insectSprite.getGlobalBounds();
    float centerX;
    float centerY = insectBounds.top + insectBounds.height / 4;
    bool isGoingRight = true;

    if (insectSprite.getScale().x < 0) { // Проверяем масштаб спрайта
        isGoingRight = false;            // Если масштаб отрицательный, значит, пчела смотрит влево       
        centerX = insectBounds.left - insectBounds.width/2;
    }
    else
    {
        centerX = insectBounds.left + insectBounds.width;
    }

    Shell::addShell(windowWidth, windowHeight, centerX, centerY, isGoingRight, this);

}


sf::FloatRect Insect::getGlobalBounds() const {
    return insectSprite.getGlobalBounds(); // Возвращает границы спрайта земли
}

bool Insect::isOffScreen(int windowWidth, int windowHeight) {
    
    position = insectSprite.getPosition();
    // Проверяем, выходит ли насекомое за пределы экрана
    if (position.y < 0 || position.y > windowHeight || position.x < 0 || position.x > windowWidth) {
        return true; // Насекомое вышло за пределы экрана
    }
    else
    {
        return false; // Насекомое на экране
    }  
    
}
