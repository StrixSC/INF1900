#pragma once
#ifndef MOTEUR_H
#define MOTEUR_H
#include "includes.h"

class Moteur {
    public:

    Moteur();
    Moteur(uint8_t left, uint8_t right);
    ~Moteur();

    uint8_t calculerPourcentage(uint8_t x);
    void startEngine();
    void changeSpeed(uint8_t left, uint8_t right);
    void reverse(uint8_t left, uint8_t right);
    void turnLeft(uint8_t speed);
    void turnRight(uint8_t speed);
    void stopEngine();

    private:
    uint8_t dutyCycleLeft_;
    uint8_t dutyCycleRight_;
    
};
#endif