#include "Engine.h"


Engine::Engine() : waspPoolSpawnTimer(0.0f), waspPoolSpawnInterval(20.0f), poolSize(10),
                   damage(1.f), resourcesLoaded(false), deltaTime(0.0f),individualSpawnTimer(0.0f), waspsToSpawn(0) {

    windowSettings();
    // Запускаем инициализацию ресурсов в отдельном потоке
    std::thread(&Engine::initializeResources, this).detach();  
}

void Engine::initializeResources() {

    loadBackgroundtexture();
    Shell::loadTextures();
    soundSettings();

   

    font.loadFromFile("assets/font.ttf");
    gameOver(font);
  
    hive = std::make_shared<Hive>(windowWidth, windowHeight, 105.f, 500.f, "assets/hive/hive.png");

    // Объявление указателей
    bee = std::make_shared<Bee>(windowWidth, windowHeight, 400.f, 300.f, *beeSound);
    insects.push_back(bee.get());
    
    // Создание контейнера с использованием shared_ptr
    targetContainer = std::make_shared<TargetContainer>(bee, hive);

    waspPool = std::make_unique<WaspPool>(poolSize, windowWidth, windowHeight, *waspSound);
    ground = std::make_unique<Ground>(195.f, 900.f, "assets/ground/branch.png");
    
    // Позиции и текстуры для каждого цветка
    flowerPositions = { {700.f, 950.f}, {1100.f, 1000.f}, {1300.f, 900.f} };
    flowerTextures = {
        "assets/flowers/flower1.png",
        "assets/flowers/flower2.png",
        "assets/flowers/flower3.png"
    };
    // Создаем отдельный объект Flower для каждой позиции и текстуры
    for (int i = 0; i < 3; i++) {
        flowers.push_back(std::make_unique<Flower>(flowerPositions[i].first, flowerPositions[i].second, flowerTextures[i]));
    }

    inventory = std::make_unique<Inventory>();

    resourcesLoaded = true; // Устанавливаем флаг, что ресурсы загружены
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
    if (targetContainer && targetContainer->isEmpty()) { // Метод проверки на пустоту
        // Удаление контейнера
        targetContainer.reset(); // Если targetContainer - это shared_ptr

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
            // Если оса вышла за пределы экрана, возвращаем её в пул
            if (wasp->isOffScreen(windowWidth, windowHeight) && (!wasp->getIsAlive() || wasp->getIsMissionCompleted())) {
                if (wasp->sound.isPlaying("waspFly")) {
                    wasp->sound.stopSound("waspFly");
                }
                insects.erase(std::remove(insects.begin(), insects.end(), wasp.get()), insects.end());                
                waspPool->returnWasp(std::move(wasp)); // Возвращаем осу в пул               
            }
        }
    }

    // Удаляем nullptr из вектора, чтобы избежать ошибок
    wasps.erase(std::remove_if(wasps.begin(), wasps.end(), [](const std::unique_ptr<Wasp>& wasp) {
        return !wasp; // Удаляем только nullptr
        }), wasps.end());
}

void Engine::waspsSpawn(float deltaTime) {

    if (wasps.empty()) { waspPoolSpawnTimer += deltaTime; }

    // Если прошло достаточно времени для начала спавна ос
    if (wasps.empty() && targetContainer && waspPoolSpawnTimer >= waspPoolSpawnInterval) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(5, 10); // Диапазон количества ос
        std::uniform_int_distribution<> spawnDelayDist(10, 25); // Задержка до следующего спавна
        waspPoolSpawnInterval = static_cast<float>(spawnDelayDist(gen));

        // Устанавливаем количество ос, которое нужно заспавнить
        waspsToSpawn = dist(gen);

        // Сбрасываем таймер
        waspPoolSpawnTimer = 0.0f;
        individualSpawnTimer = 0.0f;
    }

    // Постепенный спавн с интервалом для каждой осы
    if (waspsToSpawn > 0) {
        individualSpawnTimer += deltaTime;

        // Проверяем, если прошло достаточно времени для спавна следующей оси
        if (individualSpawnTimer >= 0.5f) { // Задержка в 0.5 секунд между осами
            float startY = 200.0f;
            float yStep = 100.0f;
            float currentYPosition = startY + (wasps.size() * yStep);
            float randomXOffset = static_cast<float>(rand() % 100 - 70);

            // Получаем осу из пула и устанавливаем позицию
            auto wasp = waspPool->getWasp();
            if (wasp) {
                wasp->setPosition(windowWidth + 500.f + randomXOffset, currentYPosition);
                wasps.push_back(std::move(wasp));
                insects.push_back(wasps.back().get());
            }

            // Обновляем количество оставшихся для спавна ос и сбрасываем индивидуальный таймер
            waspsToSpawn--;
            individualSpawnTimer = 0.0f;
        }
    }
}


void Engine::run() {

    showLoadingScreen();

    while (window.isOpen()) {

        updateBackgroundScale();
        processEvents();  // Обработка событий, включая нажатие клавиши Escape для паузы

        switch (gameState) {
        case GameState::StartMenu:
            processStartMenuEvents();  // Обработка событий стартового меню          
            renderStartMenu(font);  // Отрисовка стартового меню
            clock.restart();
            break;

        case GameState::Running:
            deltaTime = clock.restart().asSeconds();  // Обновление deltaTime только при Running
            gameUpdate(deltaTime);  // Обновление логики игры
            renderGame();  // Отрисовка игрового процесса
            break;

        case GameState::Paused:
            processPauseMenuEvents();  // Обработка событий меню паузы
            renderPauseMenu(font);  // Отрисовка меню паузы

            // Остановка звуков, если игра на паузе
            if (bee && bee->sound.isPlaying("beeFly")) {
                bee->sound.pauseSound("beeFly");
            }
            for (auto& wasp : wasps) {
                if (wasp && wasp->sound.isPlaying("waspFly")) {
                    wasp->sound.pauseSound("waspFly");
                }
            }
            clock.restart();  // Перезапуск часов при выходе из паузы
            break;

        case GameState::HowToPlay:  // Добавлен режим для экрана с инструкциями
            processHowToPlayEvents();  // Обработка событий на экране инструкций
            renderHowToPlay(font);  // Отрисовка инструкции
            clock.restart();
            break;

        default:
            break;
        }
    }
}


// Отрисовка всех объектов на экране
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


// Отрисовка меню паузы
void Engine::renderPauseMenu(sf::Font& font) {
    window.clear();

    // Фон для меню паузы
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Полупрозрачный черный фон
    window.draw(background);

    // Текст кнопок
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
                    gameState = GameState::Paused;  // Пауза

                }
                else if (gameState == GameState::Paused) {
                    gameState = GameState::Running;  // Возобновить

                }
            }
        }
    }
}



// Обработка событий меню паузы
void Engine::processPauseMenuEvents() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Координаты кнопок (указывайте их правильно, как они нарисованы в renderPauseMenu)
    sf::FloatRect continueButtonBounds(windowWidth / 2 - 50, windowHeight / 2 - 30, 100, 30);
    sf::FloatRect exitButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 30, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (continueButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::Running;  // Продолжить игру
        }
        if (exitButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            window.close();  // Выйти из игры
        }
    }
}

void Engine::processStartMenuEvents()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Координаты кнопок (указывайте их правильно, как они нарисованы в renderStartMenu)
    sf::FloatRect continueButtonBounds(windowWidth / 2 - 50, windowHeight / 2 - 30, 100, 30);
    sf::FloatRect howToPlayButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 10, 100, 30);  // Положение кнопки "How to Play"
    sf::FloatRect exitButtonBounds(windowWidth / 2 - 50, windowHeight / 2 + 50, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (continueButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::Running;  // Начать игру
            backgroundSound->playBackgroundMusic("assets/sounds/background.wav");           
        }
        if (howToPlayButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::HowToPlay;  // Переход к инструкциям
        }
        if (exitButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            window.close();  // Выйти из игры
        }
    }
}


void Engine::renderStartMenu(sf::Font& font)
{
    window.clear();

    // Фон для меню
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Полупрозрачный черный фон
    window.draw(background);

    // Текст кнопок
    sf::Text continueText("Start Game", font, 30);
    continueText.setPosition(windowWidth / 2 - continueText.getGlobalBounds().width / 2, windowHeight / 2 - 30);
    window.draw(continueText);

    sf::Text howToPlayText("How to Play", font, 30);  // Новая кнопка "How to Play"
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
        gameState = GameState::StartMenu;  // Вернуться в стартовое меню при нажатии Escape
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect backButtonBounds(windowWidth / 2 - 50, windowHeight - 50, 100, 30);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (backButtonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            gameState = GameState::StartMenu;  // Вернуться в стартовое меню
        }
    }
}

void Engine::renderHowToPlay(sf::Font& font)
{
    window.clear();

    // Фон для инструкции
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Полупрозрачный черный фон
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

    // Кнопка "Back"
    sf::Text backText("Back", font, 30);
    backText.setPosition(windowWidth / 2 - backText.getGlobalBounds().width / 2, windowHeight - 50);
    window.draw(backText);

    window.display();
}

void Engine::updateBackgroundScale()
{
    // Получаем размеры текстуры фона
    float textureWidth = static_cast<float>(backgroundTexture.getSize().x);
    float textureHeight = static_cast<float>(backgroundTexture.getSize().y);

    float scaleX = static_cast<float>(windowWidth) / textureWidth;
    float scaleY = static_cast<float>(windowHeight) / textureHeight;

    // Применяем масштаб, чтобы фон заполнил все окно
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
    gameOverText.setFont(font);  // Используйте загруженный шрифт
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);  // Установите размер шрифта
    gameOverText.setFillColor(sf::Color::White);  // Цвет текста
    gameOverText.setStyle(sf::Text::Bold);  // Сделайте текст жирным

    // Установим текст в центр экрана
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);  // Центрируем относительно середины текста
    gameOverText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);  // Центр экрана
}

void Engine::soundSettings()
{
    backgroundSound = std::make_unique<Sound>();
    beeSound = std::make_unique<Sound>();
    waspSound = std::make_unique<Sound>();

    beeSound->loadSound("beeFly", "assets/sounds/beeFly.wav");
    waspSound->loadSound("waspFly", "assets/sounds/waspFly.wav");
    backgroundSound->loadSound("background", "assets/sounds/background.wav");

    backgroundSound->setVolume(10); // Установите громкость
    beeSound->setVolume(20); // Установите громкость
    waspSound->setVolume(10); // Установите громкость
}

void Engine::showLoadingScreen() {
    // Загрузка шрифта
    loadingFont.loadFromFile("assets/font.ttf");
    // Настройка текста загрузки
    loadingText.setFont(loadingFont);
    loadingText.setString("Load..."); // Или "Load"
    loadingText.setCharacterSize(50); // Установите размер шрифта
    loadingText.setFillColor(sf::Color::White); // Установите цвет текста
    loadingText.setPosition(windowWidth - loadingText.getGlobalBounds().width - 50, windowHeight - loadingText.getGlobalBounds().height - 50);


    // Обновляем экран загрузки до тех пор, пока ресурсы не загружены
    while (!resourcesLoaded) {
        window.clear();
        window.draw(loadingText);
        window.display();
    }

}



void Engine::windowSettings()
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    windowWidth = mode.width;   // Получаем ширину
    windowHeight = mode.height;  // Получаем высоту
    window.create(mode, "Bee Collecting Nectar", sf::Style::Fullscreen); // Создаем окно
    //window.create(mode, "Bee Collecting Nectar", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

}