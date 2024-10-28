#pragma once
#include <SFML/Graphics.hpp>
#include "Bee.h"
#include "Hive.h"
#include <optional> 

enum class TargetType { None, Bee, Hive };  // ������������ ��� ����������� ���� ����

class TargetContainer {
public:
    std::shared_ptr<Bee> bee;  // ��������� �� ������ �����
    std::shared_ptr<Hive> hive; // ��������� �� ������ ����

    bool beeVisible = false;
    bool hiveVisible = false;

    TargetContainer(std::shared_ptr<Bee> b, std::shared_ptr<Hive> h)
        : bee(b), hive(h) {}

    Bee* getBee() const { return bee.get(); }
    Hive* getHive() const { return hive.get(); }

    TargetType lastTargetType = TargetType::None;  // ���������� ��� �������� ���������� ���� ����

    sf::Vector2f lastTargetPosition; // ������ ��������� ��������� ����

    bool isEmpty() const {
        return !getBee() && !getHive(); // ��� ������ ���������� ����������� ���
    }
    void removeBee() {
        bee.reset(); // �������� shared_ptr � �����
    }
    void removeHive() {
        hive.reset(); // �������� shared_ptr � �����
    }
    float detectionX = 0.0f;
    float detectionY = 0.0f;

    std::optional<sf::Vector2f> getPriorityTargetPosition(sf::Vector2f waspPosition);
   

private:
    // ������� ��� ������� ���������� ����� ����� ���������
    float getDistance(sf::Vector2f pos1, sf::Vector2f pos2) const {
        return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
    }
};


