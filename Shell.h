#pragma once
#include <SFML/Graphics.hpp>

class Insect;
class Inventory;
class Ground;
class Hive;
enum class InsectType;

class Shell {

public:
    Shell(int windowWidth, int windowHeight, float positionX, float positionY, bool isGoingRight, Insect* creator);

    void draw(sf::RenderWindow& window) const;                 
    bool isOffScreen(int windowWidth) const;                            
    void InsectCollision(Inventory& inventory, Insect& insect, float damage);
    void HiveCollision(Inventory& inventory, Hive& hive, float damage);
    void GroundCollision(Ground& ground);
    bool hasDamaged;
    static void addShell(int windowWidth, int windowHeight, float x, float y, bool isGoingRight, Insect* creator);
    static void update(float deltaTime, int windowWidth, Ground& ground, Inventory& inventory, std::vector<Insect*>& insects, Hive* hive, float damage);
    static void drawAll(sf::RenderWindow& window);
    static void loadTextures();
    Insect* creator;
    InsectType creatorType;

private:
    sf::Sprite shellSprite;   
    float speed;             
    bool goingRight;   
    static sf::Texture beeShellTexture; 
    static sf::Texture waspShellTexture; 
    static std::vector<Shell> shells;
};
