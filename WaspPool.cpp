#include "WaspPool.h"

WaspPool::WaspPool(size_t poolSize, int windowWidth, int windowHeight, Sound sound)
        : windowWidth(windowWidth), windowHeight(windowHeight), sound(sound)
{
    // ������������� ���� ��
    for (size_t i = 0; i < poolSize; ++i) {
        auto wasp = std::make_unique<Wasp>(windowWidth, windowHeight, 0.f, 0.f, sound);
        wasp->deactivate(); // ������������ ���� �� ��� ��������
        pool.push_back(std::move(wasp));
    }
}

std::unique_ptr<Wasp> WaspPool::getWasp()
{
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        if ((*it) && !(*it)->isActive()) {
            (*it)->activate(); // ���������� ���
            std::unique_ptr<Wasp> wasp = std::move(*it); // ���������� ���
            pool.erase(it); // ������� � �� ����
            return wasp; // ���������� ���
        }
    }
    return nullptr; // ���� ��� ��� �������
}

void WaspPool::returnWasp(std::unique_ptr<Wasp> wasp)
{
    if (wasp) { // ���������, ��� ��������� ������������
        wasp->deactivate(); // ������������ ���
        wasp->reset();
        pool.push_back(std::move(wasp)); // ���������� � ���          
       
    }
}
