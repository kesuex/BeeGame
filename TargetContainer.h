#pragma once
#include <SFML/Graphics.hpp>
#include "Bee.h"
#include "Hive.h"
#include <optional> 

enum class TargetType { None, Bee, Hive };  // Перечисление для обозначения типа цели

class TargetContainer {
public:
    std::shared_ptr<Bee> bee;  // Указатель на объект пчелы
    std::shared_ptr<Hive> hive; // Указатель на объект улья

    bool beeVisible = false;
    bool hiveVisible = false;

    TargetContainer(std::shared_ptr<Bee> b, std::shared_ptr<Hive> h)
        : bee(b), hive(h) {}

    Bee* getBee() const { return bee.get(); }
    Hive* getHive() const { return hive.get(); }

    TargetType lastTargetType = TargetType::None;  // Переменная для хранения последнего типа цели

    sf::Vector2f lastTargetPosition; // Хранит последнюю выбранную цель

    bool isEmpty() const {
        return !getBee() && !getHive(); // или другой логический проверочный код
    }
    void removeBee() {
        bee.reset(); // Удаление shared_ptr к пчеле
    }
    void removeHive() {
        hive.reset(); // Удаление shared_ptr к пчеле
    }
    float detectionX = 0.0f;
    float detectionY = 0.0f;

    std::optional<sf::Vector2f> getPriorityTargetPosition(sf::Vector2f waspPosition);
   

private:
    // Функция для расчёта расстояния между двумя позициями
    float getDistance(sf::Vector2f pos1, sf::Vector2f pos2) const {
        return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
    }
};


