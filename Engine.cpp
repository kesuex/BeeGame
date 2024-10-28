#include "Engine.h"


Engine::Engine() : waspPoolSpawnTimer(0.0f), waspPoolSpawnInterval(20.0f), poolSize(10),
                   damage(1.f), resourcesLoaded(false), deltaTime(0.0f),individualSpawnTimer(0.0f), waspsToSpawn(0) {

    windowSettings();
    // ��������� ������������� �������� � ��������� ������
    std::thread(&Engine::initializeResources, this).detach();  
}

void Engine::initializeResources() {

    loadBackgroundtexture();
    Shell::loadTextures();
    soundSettings();

   

    font.loadFromFile("assets/font.ttf");
    gameOver(font);
  
    hive = std::make_shared<Hive>(windowWidth, windowHeight, 105.f, 500.f, "assets/hive/hive.png");

    // ���������� ����������
    bee = std::make_shared<Bee>(windowWidth, windowHeight, 400.f, 300.f, *beeSound);
    insects.push_back(bee.get());
    
    // �������� ���������� � �������������� shared_ptr
    targetContainer = std::make_shared<TargetContainer>(bee, hive);

    waspPool = std::make_unique<WaspPool>(poolSize, windowWidth, windowHeight, *waspSound);
    ground = std::make_unique<Ground>(195.f, 900.f, "assets/ground/branch.png");
    
    // ������� � �������� ��� ������� ������
    flowerPositions = { {700.f, 950.f}, {1100.f, 1000.f}, {1300.f, 900.f} };
    flowerTextures = {
        "assets/flowers/flower1.png",
        "assets/flowers/flower2.png",
        "assets/flowers/flower3.png"
    };
    // ������� ��������� ������ Flower ��� ������ ������� � ��������
    for (int i = 0; i < 3; i++) {
        flowers.push_back(std::make_unique<Flower>(flowerPositions[i].first, flowerPositions[i].second, flowerTextures[i]));
    }

    inventory = std::make_unique<Inventory>();

    resourcesLoaded = true; // ������������� ����, ��� ������� ���������
}


void Engine::gameUpdate(float deltaTime) {

    beeUpdate(deltaTime);  
    waspsSpawn(deltaTime);
    waspsUpdate(deltaTime);
    hiveUpdate(deltaTime);
    targetContainerUpdate();

    Shell::update(deltaTime, windowWidth,*ground, *inventory, insects, hive.get(), damage);
    for (auto& flower : flowers) {
        flower->update(deltaTime);
    }
     
   
}

void Engine::targetContainerUpdate()
{
    if (targetContainer && targetContainer->isEmpty()) { // ����� �������� �� �������
        // �������� ����������
        targetContainer.reset(); // ���� targetContainer - ��� shared_ptr

    }
}

void Engine::hiveUpdate(float deltaTime)
{
    if (hive) {
        hive->update(deltaTime);
        if (hive->isOffScreen(windowWidth, windowHeight)){
        targetContainer->removeHive();
        hive.reset();
         }
    }
    
}

void Engine::beeUpdate(float deltaTime)
{
    if (bee) {
        bee->Update(deltaTime, *ground, flowers, hive.get(), *inventory);
        if (bee->isOffScreen(windowWidth, windowHeight) && !bee->getIsAlive()) {    
            insects.erase(std::remove(insects.begin(), insects.end(), bee.get()), insects.end());
            targetContainer->removeBee();
            bee.reset();             
        }
    }
}

void Engine::waspsUpdate(float deltaTime)
{
    for (auto& wasp : wasps) {
        if (wasp) {
            wasp->Update(deltaTime, *ground, targetContainer.get());
            // ���� ��� ����� �� ������� ������, ���������� � � ���
            if (wasp->isOffScreen(windowWidth, windowHeight) && (!wasp->getIsAlive() || wasp->getIsMissionCompleted())) {
                if (wasp->sound.isPlaying("waspFly")) {
                    wasp->sound.stopSound("waspFly");
                }
                insects.erase(std::remove(insects.begin(), insects.end(), wasp.get()), insects.end());                
                waspPool->returnWasp(std::move(wasp)); // ���������� ��� � ���               
            }
        }
    }

    // ������� nullptr �� �������, ����� �������� ������
    wasps.erase(std::remove_if(wasps.begin(), wasps.end(), [](const std::unique_ptr<Wasp>& wasp) {
        return !wasp; // ������� ������ nullptr
        }), wasps.end());
}

void Engine::waspsSpawn(float deltaTime) {

    if (wasps.empty()) { waspPoolSpawnTimer += deltaTime; }

    // ���� ������ ���������� ������� ��� ������ ������ ��
    if (wasps.empty() && targetContainer && waspPoolSpawnTimer >= waspPoolSpawnInterval) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(5, 10); // �������� ���������� ��
        std::uniform_int_distribution<> spawnDelayDist(10, 25); // �������� �� ���������� ������
        waspPoolSpawnInterval = static_cast<float>(spawnDelayDist(gen));

        // ������������� ���������� ��, ������� ����� ����������
        waspsToSpawn = dist(gen);

        // ���������� ������
        waspPoolSpawnTimer = 0.0f;
        individualSpawnTimer = 0.0f;
    }

    // ����������� ����� � ���������� ��� ������ ���
    if (waspsToSpawn > 0) {
        individualSpawnTimer += deltaTime;

        // ���������, ���� ������ ���������� ������� ��� ������ ��������� ���
        if (individualSpawnTimer >= 0.5f) { // �������� � 0.5 ������ ����� �����
            float startY = 200.0f;
            float yStep = 100.0f;
            float currentYPosition = startY + (wasps.size() * yStep);
            float randomXOffset = static_cast<float>(rand() % 100 - 70);

            // �������� ��� �� ���� � ������������� �������
            auto wasp = waspPool->getWasp();
            if (wasp) {
                wasp->setPosition(windowWidth + 500.f + randomXOffset, currentYPosition);
                wasps.push_back(std::move(wasp));
                insects.push_back(wasps.back().get());
            }

            // ��������� ���������� ���������� ��� ������ �� � ���������� �������������� ������
            waspsToSpawn--;
            individualSpawnTimer = 0.0f;
        }
    }
}


void Engine::run() {

    showLoadingScreen();

    while (window.isOpen()) {

        updateBackgroundScale();
        processEvents();  // ��������� �������, ������� ������� ������� Escape ��� �����

        switch (gameState) {
        case GameState::StartMenu:
            processStartMenuEvents();  // ��������� ������� ���������� ����          
            renderStartMenu(font);  // ��������� ���������� ����
            clock.restart();
            break;

        case GameState::Running:
            deltaTime = clock.restart().asSeconds();  // ���������� deltaTime ������ ��� Running
            gameUpdate(deltaTime);  // ���������� ������ ����
            renderGame();  // ��������� �������� ��������
            break;

        case GameState::Paused:
            processPauseMenuEvents();  // ��������� ������� ���� �����
            renderPauseMenu(font);  // ��������� ���� �����

            // ��������� ������, ���� ���� �� �����
            if (bee && bee->sound.isPlaying("beeFly")) {
                bee->sound.pauseSound("beeFly");
            }
            for (auto& wasp : wasps) {
                if (wasp && wasp->sound.isPlaying("waspFly")) {
                    wasp->sound.pauseSound("waspFly");
                }
            }
            clock.restart();  // ���������� ����� ��� ������ �� �����
            break;

        case GameState::HowToPlay:  // �������� ����� ��� ������ � ������������
            processHowToPlayEvents();  // ��������� ������� �� ������ ����������
            renderHowToPlay(font);  // ��������� ����������
            clock.restart();
            break;

        default:
            break;
        }
    }
}


// ��������� ���� �������� �� ������
void Engine::renderGame() {
    window.clear();
   
    window.draw(backgroundSprite);  
    window.draw(treeSprite);
    ground->draw(window);

    for (auto& flower : flowers) {
        flower->draw(window);
    }
    if(hive)
    {
      hive->draw(window);
    }    

    if (bee) {
        bee->draw(window);
    }
   
    for (auto& wasp : wasps) {
        if (wasp) {
            wasp->draw(window);
        }       
    }

    Shell::drawAll(window);
    inventory->draw(window);

    if(!bee){
        window.draw(gameOverText);  
    }
    window.display();
}


// ��������� ���� �����
void Engine::renderPauseMenu(sf::Font& font) {
    window.clear();

    // ��� ��� ���� �����
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // �������������� ������ ���
    window.draw(background);

    // ����� ������
    sf::Text continueText("Continue", font, 30);
    continueText.setPosition(windowWidth / 2 - continueText.getGlobalBounds().width / 2, windowHeight / 2 - 30);
    window.draw(continueText);

    sf::Text exitText("Exit", font, 30);
    exitText.setPosition(windowWidth / 2 - exitText.getGlobalBounds().width / 2, windowHeight / 2 + 30);
    window.draw(exitText);
    window.display();

}


void Engine::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (gameState == GameState::Running) {
                    gameState = GameState::Paused;  // �����

                }
                else if (gameState == GameState::Paused) {
                    gameState = GameState::Running;  // �����������

                }
            }
        }
    }
}



// ��������� ������� ���� �����
void Engine::processPauseMenuEvents() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // ���������� ������ (���������� �� ���������, ��� ��� ���������� � renderPauseMenu)
    sf::FloatRect continueButtonBounds(windowWidth / 2 - 50, windowHeight / 2 - 30, 100, 30);
    sf::FloatRect exitButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 30, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (continueButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::Running;  // ���������� ����
        }
        if (exitButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            window.close();  // ����� �� ����
        }
    }
}

void Engine::processStartMenuEvents()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // ���������� ������ (���������� �� ���������, ��� ��� ���������� � renderStartMenu)
    sf::FloatRect continueButtonBounds(windowWidth / 2 - 50, windowHeight / 2 - 30, 100, 30);
    sf::FloatRect howToPlayButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 10, 100, 30);  // ��������� ������ "How to Play"
    sf::FloatRect exitButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 50, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (continueButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::Running;  // ������ ����
            backgroundSound->playBackgroundMusic("assets/sounds/background.wav");           
        }
        if (howToPlayButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::HowToPlay;  // ������� � �����������
        }
        if (exitButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            window.close();  // ����� �� ����
        }
    }
}


void Engine::renderStartMenu(sf::Font& font)
{
    window.clear();

    // ��� ��� ����
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // �������������� ������ ���
    window.draw(background);

    // ����� ������
    sf::Text continueText("Start Game", font, 30);
    continueText.setPosition(windowWidth / 2 - continueText.getGlobalBounds().width / 2, windowHeight / 2 - 30);
    window.draw(continueText);

    sf::Text howToPlayText("How to Play", font, 30);  // ����� ������ "How to Play"
    howToPlayText.setPosition(windowWidth / 2 - howToPlayText.getGlobalBounds().width / 2, windowHeight / 2 + 10);
    window.draw(howToPlayText);

    sf::Text exitText("Exit", font, 30);
    exitText.setPosition(windowWidth / 2 - exitText.getGlobalBounds().width / 2, windowHeight / 2 + 50);
    window.draw(exitText);

    window.display();
}


void Engine::processHowToPlayEvents()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        gameState = GameState::StartMenu;  // ��������� � ��������� ���� ��� ������� Escape
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect backButtonBounds(windowWidth / 2 - 50, windowHeight - 50, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (backButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::StartMenu;  // ��������� � ��������� ����
        }
    }
}

void Engine::renderHowToPlay(sf::Font& font)
{
    window.clear();

    // ��� ��� ����������
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // �������������� ������ ���
    window.draw(background);

    sf::Text instructionsText(
        "Move with WASD\n"
        "Fly to a flower and collect nectar by pressing E\n"
        "Repair the damaged hive with collected nectar by pressing Q\n"
        "Shoot nectar at enemies with the left mouse button\n"
        "The bee can fall into a pit if it runs out of energy\n"
        "Energy is restored when landing on solid objects", 

        font, 24);


    instructionsText.setPosition(windowWidth / 2 - instructionsText.getGlobalBounds().width / 2, windowHeight / 2 - 100);
    window.draw(instructionsText);

    // ������ "Back"
    sf::Text backText("Back", font, 30);
    backText.setPosition(windowWidth / 2 - backText.getGlobalBounds().width / 2, windowHeight - 50);
    window.draw(backText);

    window.display();
}

void Engine::updateBackgroundScale()
{
    // �������� ������� �������� ����
    float textureWidth = static_cast<float>(backgroundTexture.getSize().x);
    float textureHeight = static_cast<float>(backgroundTexture.getSize().y);

    float scaleX = static_cast<float>(windowWidth) / textureWidth;
    float scaleY = static_cast<float>(windowHeight) / textureHeight;

    // ��������� �������, ����� ��� �������� ��� ����
    backgroundSprite.setScale(scaleX, scaleY);
}


void Engine::loadBackgroundtexture()
{
    backgroundTexture.loadFromFile("assets/background.jpg");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    treeTexture.loadFromFile("assets/hive/tree.png");
    treeSprite.setTexture(treeTexture);
    treeSprite.setPosition(105, 500);
    treeSprite.setOrigin(treeSprite.getGlobalBounds().width / 2, treeSprite.getGlobalBounds().height / 2);
}



void Engine::gameOver(sf::Font& font)
{
    gameOverText.setFont(font);  // ����������� ����������� �����
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);  // ���������� ������ ������
    gameOverText.setFillColor(sf::Color::White);  // ���� ������
    gameOverText.setStyle(sf::Text::Bold);  // �������� ����� ������

    // ��������� ����� � ����� ������
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);  // ���������� ������������ �������� ������
    gameOverText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);  // ����� ������
}

void Engine::soundSettings()
{
    backgroundSound = std::make_unique<Sound>();
    beeSound = std::make_unique<Sound>();
    waspSound = std::make_unique<Sound>();

    beeSound->loadSound("beeFly", "assets/sounds/beeFly.wav");
    waspSound->loadSound("waspFly", "assets/sounds/waspFly.wav");
    backgroundSound->loadSound("background", "assets/sounds/background.wav");

    backgroundSound->setVolume(10); // ���������� ���������
    beeSound->setVolume(20); // ���������� ���������
    waspSound->setVolume(10); // ���������� ���������
}

void Engine::showLoadingScreen() {
    // �������� ������
    loadingFont.loadFromFile("assets/font.ttf");
    // ��������� ������ ��������
    loadingText.setFont(loadingFont);
    loadingText.setString("Load..."); // ��� "Load"
    loadingText.setCharacterSize(50); // ���������� ������ ������
    loadingText.setFillColor(sf::Color::White); // ���������� ���� ������
    loadingText.setPosition(windowWidth - loadingText.getGlobalBounds().width - 50, windowHeight - loadingText.getGlobalBounds().height - 50);


    // ��������� ����� �������� �� ��� ���, ���� ������� �� ���������
    while (!resourcesLoaded) {
        window.clear();
        window.draw(loadingText);
        window.display();
    }

}



void Engine::windowSettings()
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    windowWidth = mode.width;   // �������� ������
    windowHeight = mode.height;  // �������� ������
    window.create(mode, "Bee Collecting Nectar", sf::Style::Fullscreen); // ������� ����
    //window.create(mode, "Bee Collecting Nectar", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

}