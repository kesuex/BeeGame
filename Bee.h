#pragma once
#include "Insect.h"
#include "Hive.h"



class Bee : public Insect
{
public:

    Bee(int windowWidth, int windowHeight, float positionX, float positionY, Sound sound);

    void Update(float deltaTime, Ground& ground, std::vector<std::unique_ptr<Flower>>& flowers, Hive* hive, Inventory& inventory);
    unsigned int getPollenCount(Inventory& inventory) const;

private:
    bool isKeyPressedLastFrame;
    bool isOnHive;
    bool isOnFlower; // Флаг, что пчела на цветке

    void die(float deltaTime) override;
    void animatedWings() override;
    void animatedWalk() override;
    void takeDamage(Inventory& inventory, float damage) override;

    void givePollen(Inventory& inventory, Hive* hive);
    void dangerSituation(Inventory& inventory, int windowHeight);
    void updateEnergy(Inventory& inventory, float deltaTime); 
    void shoot(Inventory& inventory);
    void movement(float deltaTime);
    void globalBoarders();
    void tryShoot(Inventory& Inventory);
    bool flowerCollision(Flower& flower);
    bool hiveCollision(Hive* hive);
    void collectPollen(Flower& flower, Inventory& inventory);


};

