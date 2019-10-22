#pragma once

#ifndef TIMER_H
#define TIMER_H
#include "includes.h"
#include "enums.h"


 
// Comment initier les timers:
// Timer (MODE , TIMER QUE L'ON VEUT UTILISER, DUREE DU TIMER, PRESCALER)
// CHOIX DE TIMERS:     TCNT0_(8Bits), TCNT1_ (16Bits), TCNT2_(8Bits)
// CHOIX DE MODES:      Normal, PWM_PC, CTC, FPWM_8
// CHOIX DE PRESCALERS: P1, P8, P64, P256, P1024

// Exemple: Timer timer(Normal, TCNT1_, d, P1024);


class Timer {

public:

    Timer();
    Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode);
    ~Timer();
    void start();
    void stop();

    
    void setPrescaler(Prescaler p);
    void setDuree(uint8_t duree); 
    void setTimer(Timers t);
    void setInitMode(Mode m);
    void setOutputMode(OutputMode m);

    void PWM(uint8_t left, uint8_t right);
private:
    Timers timer_;
    Mode mode_;
    Prescaler prescaler_;
    uint8_t duree_;
    OutputMode outputMode_;
};

#endif TIMER_H