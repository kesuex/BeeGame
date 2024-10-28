#include "Insect.h"

Insect::Insect(int windowWidth, int windowHeight, float positionX, float positionY, InsectType type, Sound sound)
    : windowWidth(windowWidth), windowHeight(windowHeight), positionX(positionX), positionY(positionY), type(type), sound(sound),
      wingState(0),walkState(0), animationTime(0.1f), speed(0), isMoving(false), isOnGround(false),isAlive(true), isGroundCollision(false),
      isFlying(false),canShoot(false), gravity(200.0f), verticalSpeed(0.0f), fallSpeed(200.0f) {}

      
void Insect:: checkIfMoving() {
    // �������� ������� �������
    sf::Vector2f currentPosition = insectSprite.getPosition();

    // ���� ������� ������� ���������� �� ����������
    if (currentPosition != previousPosition) {
        isMoving = true;  // ��������� ���������
    }
    else {
        isMoving = false; // ��������� ������������
    }

    // ��������� ���������� ������� ��� ��������� ��������
    previousPosition = currentPosition;
}


void Insect::checkGroundCollision(Ground& ground)
{
    // �������� ������� �����, �������� �������� ����� origin � ������
    insectBounds = insectSprite.getGlobalBounds();
    groundBounds = ground.getGlobalBounds();

    // ������������ beeBounds, ��� ��� ����� origin ����������� � ����� ��������
    insectBounds.left = insectSprite.getPosition().x - insectBounds.width / 2;
    insectBounds.top = insectSprite.getPosition().y - insectBounds.height / 2;

    // ���� ��������� ������������
    if (checkSATCollision(insectBounds, groundBounds)) {
        // �������� ������������ ������
        if (insectBounds.top + insectBounds.height <= groundBounds.top + 1) {  // �������� �� �����������
            insectSprite.setPosition(insectSprite.getPosition().x, groundBounds.top - insectBounds.height / 2);
            isOnGround = true;
            isFlying = false;
        }

        // �������� ������������ �����
        else if (insectBounds.top >= groundBounds.top + groundBounds.height - 1) {  // �������� �� �����������
            insectSprite.setPosition(insectSprite.getPosition().x, groundBounds.top + groundBounds.height + insectBounds.height / 2);          
            isFlying = true;
        }


        // �������� ������������ �����
        else if (insectBounds.left + insectBounds.width >= groundBounds.left && insectBounds.left <= groundBounds.left) {
            insectSprite.setPosition(groundBounds.left - insectBounds.width / 2, insectSprite.getPosition().y);            
            isFlying = true;
        }

        // �������� ������������ ������
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
        // ���������� ������� �����
        float groundLeft = groundBounds.left;
        float groundRight = groundBounds.left + groundBounds.width;

        // �������� ������ �����
        float beeHalfWidth = insectBounds.width / 2;

        // ���������, ������� �� ����� ������� ����� �� ������� ����� �� �������� �� ������
        if (insectBounds.left < groundLeft - beeHalfWidth) {
            // ����� ������� ����� �������
            isOnGround = false;  // ���������� �������, ��� �� �����
            isFlying = true;     // ����� ������ ������
        }
        // ���������, ������� �� ������ ������� ����� �� ������� ����� �� �������� �� ������
        else if (insectBounds.left + insectBounds.width > groundRight + beeHalfWidth) {
            // ������ ������� ����� �������
            isOnGround = false;  // ���������� �������, ��� �� �����
            isFlying = true;     // ����� ������ ������
        }
    }

    
}

bool Insect::isSeparatingAxis(float minA, float maxA, float minB, float maxB) {
    return (maxA < minB || maxB < minA);
}

// ���������� ������ �������� ������������ � �������������� SAT
bool Insect::checkSATCollision(const sf::FloatRect& rectA, const sf::FloatRect& rectB) {
    // �������� �� ��� X
    if (isSeparatingAxis(rectA.left, rectA.left + rectA.width, rectB.left, rectB.left + rectB.width)) {
        return false; // ��� ������������ �� ��� X
    }

    // �������� �� ��� Y
    if (isSeparatingAxis(rectA.top, rectA.top + rectA.height, rectB.top, rectB.top + rectB.height)) {
        return false; // ��� ������������ �� ��� Y
    }

    // ��� ����������� ���, ������ ������������ ���������
    return true;
}

void Insect::animatedWings()
{
    if (animationClock.getElapsedTime().asSeconds() > animationTime) {
        wingState = (wingState + 1) % wingTextures.size();  // ����������� ����� ���������
        insectSprite.setTexture(wingTextures[wingState]);      // ��������� ����� ��������
        animationClock.restart();                          
    }

}

void Insect::animatedWalk()
{
    if (animationClock.getElapsedTime().asSeconds() > animationTime) {
        walkState = (walkState + 1) % walkTextures.size();  // ����������� ����� ���������
        insectSprite.setTexture(walkTextures[walkState]);      // ��������� ����� ��������
        animationClock.restart();
    }

}


void Insect::die(float deltaTime)
{
    insectSprite.move(0, fallSpeed * deltaTime);
    
}



// ��������� ����� �� ������
void Insect::draw(sf::RenderWindow& window) {
    wingSprite.setTexture(wingTextures[wingState]);
    wingSprite.setPosition(insectSprite.getPosition());  // ������������� ������� �� �� ��, ��� � � ����
    window.draw(insectSprite);   // ��������� ���� �����
    //window.draw(wingSprite);   // ��������� �������
}



sf::Vector2f Insect::getPosition() const
{ 
   return insectSprite.getPosition();  
}



void Insect::shellInitialization() {

    // ����������� �����������: ���� ����� ������� �����, �������� false
    insectBounds = insectSprite.getGlobalBounds();
    float centerX;
    float centerY = insectBounds.top + insectBounds.height / 4;
    bool isGoingRight = true;

    if (insectSprite.getScale().x < 0) { // ��������� ������� �������
        isGoingRight = false;            // ���� ������� �������������, ������, ����� ������� �����       
        centerX = insectBounds.left - insectBounds.width/2;
    }
    else
    {
        centerX = insectBounds.left + insectBounds.width;
    }

    Shell::addShell(windowWidth, windowHeight, centerX, centerY, isGoingRight, this);

}


sf::FloatRect Insect::getGlobalBounds() const {
    return insectSprite.getGlobalBounds(); // ���������� ������� ������� �����
}

bool Insect::isOffScreen(int windowWidth, int windowHeight) {
    
    position = insectSprite.getPosition();
    // ���������, ������� �� ��������� �� ������� ������
    if (position.y < 0 || position.y > windowHeight || position.x < 0 || position.x > windowWidth) {
        return true; // ��������� ����� �� ������� ������
    }
    else
    {
        return false; // ��������� �� ������
    }  
    
}
