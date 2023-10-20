//
// Created by zrk on 2023/10/20.
//

#ifndef DOP_TICK_H
#define DOP_TICK_H
#include <chrono>
#include <string>

class Tick
{
private:
    std::string context;
    std::chrono::time_point<std::chrono::steady_clock> _start;

public:
    void Start(std::string contextName="");
    void End(double scale=1.0);
};


#endif //DOP_TICK_H
