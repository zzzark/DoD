//
// Created by zrk on 2023/10/20.
//

#include "tick.h"
#include <iostream>

void Tick::Start(std::string contextName) {
    context = contextName;
    if (!context.empty()) {
        std::cout << ">>>>>> " << context << std::endl;
    }
    _start = std::chrono::high_resolution_clock::now();
}

void Tick::End(double scale)
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = end - _start;
    double deltaTime = delta.count();
    deltaTime *= scale;

    std::cout << "  " << deltaTime << std::endl;
    if (!context.empty()) {
        std::cout << "<<<<<<  " << std::endl << std::endl;
    }
}
