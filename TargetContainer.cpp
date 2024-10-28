#include "TargetContainer.h"

 // �� �������� �������� ��� ����������, ���� ����������� std::optional

std::optional<sf::Vector2f> TargetContainer::getPriorityTargetPosition(sf::Vector2f waspPosition)
{
    // �������������� ���������� �� ����� � ����
    float beeDistance = std::numeric_limits<float>::max();
    float hiveDistance = std::numeric_limits<float>::max();

 

    // ���������, ����� �� �����
    if (bee) {
        beeDistance = getDistance(waspPosition, bee->getPosition());
        beeVisible = (beeDistance < 500.0f); // ������ ��������� �����
    }

    // ���������, ����� �� ����
    if (hive) {
        hiveDistance = getDistance(waspPosition, hive->getPosition());
        hiveVisible = (hiveDistance < 700.0f); // ������ ��������� ����
    }

    // ������ ������ ����
    if (bee && hive && beeVisible && hiveVisible &&
        bee->getIsAlive() && !hive->getDestroyed()) {
        
        detectionX = 700.f;
        detectionY = 500.f;
        return hive->getPosition();
        
      
    }
    else if (bee && bee->getIsAlive() && beeVisible) {
        // ���� ����� ������ �����
        detectionX = 500.f;
        detectionY = 70.f;
        return bee->getPosition();
    }
    else if (hive && !hive->getDestroyed() && hiveVisible) {
        // ���� ����� ������ ����
        detectionX = 700.f;
        detectionY = 500.f;
        return hive->getPosition();
    }

    // ���������� std::nullopt, ���� ������ �� �����
    return std::nullopt;
}


