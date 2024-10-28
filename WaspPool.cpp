#include "WaspPool.h"

WaspPool::WaspPool(size_t poolSize, int windowWidth, int windowHeight, Sound sound)
        : windowWidth(windowWidth), windowHeight(windowHeight), sound(sound)
{
    // Инициализация пула ос
    for (size_t i = 0; i < poolSize; ++i) {
        auto wasp = std::make_unique<Wasp>(windowWidth, windowHeight, 0.f, 0.f, sound);
        wasp->deactivate(); // Деактивируем всех ос при создании
        pool.push_back(std::move(wasp));
    }
}

std::unique_ptr<Wasp> WaspPool::getWasp()
{
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        if ((*it) && !(*it)->isActive()) {
            (*it)->activate(); // Активируем осу
            std::unique_ptr<Wasp> wasp = std::move(*it); // Перемещаем осу
            pool.erase(it); // Удаляем её из пула
            return wasp; // Возвращаем осу
        }
    }
    return nullptr; // Если все осы активны
}

void WaspPool::returnWasp(std::unique_ptr<Wasp> wasp)
{
    if (wasp) { // Проверяем, что указатель действителен
        wasp->deactivate(); // Деактивируем осу
        wasp->reset();
        pool.push_back(std::move(wasp)); // Возвращаем в пул          
       
    }
}
