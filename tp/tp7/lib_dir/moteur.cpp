#include "moteur.h"
/*
@Brief: permet de controller le moteur, selon differentes vitesse 
@Param: uin8_t left et uint8_t right
@Return: void
*/
Moteur::Moteur(){
}

Moteur::Moteur(uint8_t left, uint8_t right){
    dutyCycleLeft_ = calculerPourcentage(left);
    dutyCycleRight_ = calculerPourcentage(right);  
}

Moteur::~Moteur(){}

uint8_t Moteur::calculerPourcentage(uint8_t x){
    // 255 = duree maximal (8bit)
    // donc on fait Max - (pourcentage du max, qui est le pourcentage que l'on veut attribue au PWM des moteurs)
    // Par exemple, si l'on fournit un pourcentage de x = 50, on aura un duty_cycle de 125 (50%).
    return ( 255 - ( 255 * x/100 ) );                   
}

void Moteur::startEngine(){
    TCNT1 = 0;          
    TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM10);  
    TCCR1B = _BV(WGM10);
    TCCR1C = 0;
}

void Moteur::changeSpeed(uint8_t left, uint8_t right){
    startEngine();
    dutyCycleLeft_ = calculerPourcentage(left);
    dutyCycleRight_ = calculerPourcentage(right);
    OCR1A = dutyCycleLeft_;
    OCR1B = dutyCycleRight_;
}