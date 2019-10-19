#pragma once

#ifndef TIMER_H
#define TIMER_H
#include "includes.h"
#include "timer_enums.h"

class Timer {

public:

    Timer();
    Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler);
    ~Timer();
    void start();
    void stop();

    void setPrescaler(Prescaler p);
    void setDuree(uint8_t duree); 
    void setTimer(Timers t);
    void setMode(Mode m);


private:
    Timers timer_;
    Mode mode_;
    Prescaler prescaler_;
    uint8_t duree_;
};

#endif TIMER_H