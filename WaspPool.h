#pragma once
#include <vector>
#include <iostream>
#include "Wasp.h"  

class WaspPool {
public:
   
    WaspPool(size_t poolSize, int windowWidth, int windowHeight, Sound sound);
       
    size_t getPoolSize() const { return pool.size(); }
    // Получаем неактивную осу из пула
    std::unique_ptr<Wasp>getWasp();
    // Возвращаем осу в пул (деактивируем)
    void returnWasp(std::unique_ptr<Wasp> wasp);

private:
    std::vector<std::unique_ptr<Wasp>> pool; // Вектор пула ос
    int windowWidth;
    int windowHeight;
    Sound sound; 
};
