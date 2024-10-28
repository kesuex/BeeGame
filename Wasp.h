#pragma once
#include "Insect.h"
#include "Bee.h" 
#include "Hive.h"
#include "TargetContainer.h"

class Wasp : public Insect
{
public:
    
    Wasp(int windowWidth, int windowHeight, float positionX, float positionY, Sound sound);  

    void setPosition(float x, float y);
    void Update(float deltaTime, Ground& ground, TargetContainer* targetContainer);

    //WaspPool functions
    void activate() { isActiveFlag = true; }
    void deactivate() { isActiveFlag = false; }
    bool isActive() const { return isActiveFlag; }
    void reset() { isAlive = true; startSpawn = true; }
    bool getIsMissionCompleted() const { return missionCompleted; }

private:
    void takeDamage() override;
    void animatedWings() override;
    void animatedWalk() override;
    void die(float deltaTime) override;

    void shoot();
    void moveOnTheField(float deltaTime);
    void moveOutTheField(float deltaTime);
    void attack(float deltaTime, TargetContainer* targetContainer);
    bool setAttackPosition(float deltaTime, TargetContainer* targetContainer);
    void calculateTargetDiretion(TargetContainer* targetContainer);
    void movement(float deltaTime);
    void randomizeDirection(bool isOnGround); // ћетод дл€ генерации случайного направлени€
    void patrol(float deltaTime);

    bool hasAlreadyCheckedLiveCondition = false;
    bool isBeeAlive; 
    bool isHiveDestroyed; 
    bool isActiveFlag;  
    bool detectPlayer(TargetContainer* targetContainer);
    void checkedIsTargetAlive(TargetContainer* targetContainer, bool& detected);
    bool isPatroling;
    bool isPositionSet;
    bool startSpawn;
    bool missionCompleted;
    float targetXonTheField;
    float targetXoutTheField;
    float moveDuration;        
    float pauseDuration;       
    float shootCooldownTime;

    sf::Clock shootCooldownClock; // ѕеременна€ дл€ отслеживани€ времени между выстрелами 
    sf::Vector2f velocity;
    sf::Vector2f direction;    
    sf::Clock movementClock;    
    sf::Clock pauseClock;
    sf::Vector2f targetPosition;
    sf::Vector2f attackPosition;

   
};

