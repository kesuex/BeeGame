#include "TargetContainer.h"

 // Не забудьте включить эту библиотеку, если используете std::optional

std::optional<sf::Vector2f> TargetContainer::getPriorityTargetPosition(sf::Vector2f waspPosition)
{
    // Инициализируем расстояния до пчелы и улья
    float beeDistance = std::numeric_limits<float>::max();
    float hiveDistance = std::numeric_limits<float>::max();

 

    // Проверяем, видна ли пчела
    if (bee) {
        beeDistance = getDistance(waspPosition, bee->getPosition());
        beeVisible = (beeDistance < 500.0f); // Радиус видимости пчелы
    }

    // Проверяем, виден ли улей
    if (hive) {
        hiveDistance = getDistance(waspPosition, hive->getPosition());
        hiveVisible = (hiveDistance < 700.0f); // Радиус видимости улья
    }

    // Логика выбора цели
    if (bee && hive && beeVisible && hiveVisible &&
        bee->getIsAlive() && !hive->getDestroyed()) {
        
        detectionX = 700.f;
        detectionY = 500.f;
        return hive->getPosition();
        
      
    }
    else if (bee && bee->getIsAlive() && beeVisible) {
        // Если видна только пчела
        detectionX = 500.f;
        detectionY = 70.f;
        return bee->getPosition();
    }
    else if (hive && !hive->getDestroyed() && hiveVisible) {
        // Если виден только улей
        detectionX = 700.f;
        detectionY = 500.f;
        return hive->getPosition();
    }

    // Возвращаем std::nullopt, если ничего не видно
    return std::nullopt;
}


