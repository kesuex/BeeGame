#pragma once
#include <SFML/Graphics.hpp>
#include "Bee.h"
#include "Wasp.h"
#include "Ground.h"
#include "Flower.h"
#include "Inventory.h"
#include "Hive.h"
#include "Sound.h"
#include "WaspPool.h"
#include <random> 
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread> // ��� ������ � ��������

// ��������� ����
enum class GameState {
    StartMenu,
    HowToPlay,
    Running,
    Paused
};

class Engine
{
public:
 
    Engine();

    void windowSettings();
    void loadBackgroundtexture();
    void run();
   
private:
    GameState gameState = GameState::StartMenu;  // ��������� ����
    float individualSpawnTimer;
    int waspsToSpawn;
    bool resourcesLoaded; // ���������� ��� ������������ ��������� ��������  
    float waspPoolSpawnTimer;
    float waspPoolSpawnInterval;
    size_t poolSize;
    int windowWidth;
    int windowHeight;
    float damage;
    float deltaTime;

    void initializeResources();
    void gameOver(sf::Font& font);
    void soundSettings();
    void showLoadingScreen();
    void processEvents();
    void gameUpdate(float deltaTime);
    void targetContainerUpdate();
    void hiveUpdate(float deltaTime);
    void beeUpdate(float deltaTime);
    void waspsUpdate(float deltaTime);
    void waspsSpawn(float deltaTime);  
    void renderGame();  
    void updateBackgroundScale();
    void processPauseMenuEvents();
    void processStartMenuEvents();  
    void processHowToPlayEvents();
    void renderHowToPlay(sf::Font& font);
    void renderPauseMenu(sf::Font& font);
    void renderStartMenu(sf::Font& font);

   
    
    std::vector<std::unique_ptr<Flower>> flowers;
    std::vector<std::pair<float, float>> flowerPositions; 
    std::vector<std::string> flowerTextures; 

    std::vector<Insect*> insects;
    std::vector<std::unique_ptr<Wasp>> wasps;
    std::unique_ptr<WaspPool> waspPool; 

    std::unique_ptr<Ground> ground;
    std::unique_ptr<Inventory> inventory;
    std::unique_ptr<Sound> backgroundSound;
    std::unique_ptr<Sound> beeSound;
    std::unique_ptr<Sound> waspSound;

    std::shared_ptr<TargetContainer> targetContainer;
    std::shared_ptr<Hive> hive;
    std::shared_ptr<Bee> bee;

    sf::Texture backgroundTexture;  // �������� ����
    sf::Sprite backgroundSprite;     // ������ ����
    sf::Texture treeTexture;  // �������� ����
    sf::Sprite treeSprite;     // ������ ����

    sf::RenderWindow window;    // ���� ����
    sf::Clock clock;            // ���� ��� ������� ������� ����� �������

    // �������� ������
    sf::Font font;
    sf::Font loadingFont; // ����� ��� ������ ��������
    sf::Text loadingText; // ����� ��������
    sf::Text gameOverText;

};

