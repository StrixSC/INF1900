#ifndef TIMER_H
#define TIMER_H
#include "includes.h"

enum Timers { Timer8bit_1, Timer8bit_2, Timer16_bit };
enum Mode {Normal, PWM_PC, CTC, FPWM_8};
enum Prescaler {1, 8, 64, 256, 1024}

class Timer {

public:

    Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler);
    ~Timer();
    start();
    stop();


    setPrescaler();
    setDuree(); 
    setTimer();
    setMode();


private:
    Timers timer_ = Timer8bit_1;
    Mode mode_ = Normal;
    Prescaler prescaler_ = 1;
    uint8_t duree_ = 0;
    uint8_t minuterieStart = 0;
    uint8_t minuterieExpiree = 0;
};

#endif TIMER_H