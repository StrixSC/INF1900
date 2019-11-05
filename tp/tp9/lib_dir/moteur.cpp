#include "moteur.h"

/*
@Brief: Constructeur par defaut de la classe Moteur. 
@Param: void
@Return: void
*/
Moteur::Moteur(){
}

/*
@Brief: Constructeur par param de la class Moteur
@Param: une valeur 8 bit pour la puissance de la roue gauche, une valeur 8 bit pour la puissance de la roue droite
@Return: void
*/
Moteur::Moteur(uint8_t left, uint8_t right){
    dutyCycleLeft_ = calculerPourcentage(left);
    dutyCycleRight_ = calculerPourcentage(right);  
}

/*
@Brief: Destructeur de la classe moteur
@Param: void
@Return: void
*/
Moteur::~Moteur(){}

/*
@Brief: Cette methode permet de convertir le pourcentage de dutyCycle fournit par l'usager
en valeur de 8bit qui sera facile a lire par le moteur. 
@Param: une valeur de 8bit (uint_8t)
@Return: une valeur en 8bit.(uint_8t)
*/
uint8_t Moteur::calculerPourcentage(uint8_t x){
    // 255 = duree maximal (8bit)
    // donc on fait Max - (pourcentage du max, qui est le pourcentage que l'on veut attribue au PWM des moteurs)
    // Par exemple, si l'on fournit un pourcentage de x = 50, on aura un duty_cycle de 125 (50%).
    return ( 255 - ( 255 * x/100 ) );                   
}

/*
@Brief: Cette methode set le moteur en mode High-Set et en mode PhaseCorrect
Aussi, elle set le timer1 a 0 avec un prescaler de 8.
@Param: une valeur de 8bit (uint_8t)
@Return: une valeur en 8bit.(uint_8t)
*/
void Moteur::startEngine(){
    TCNT1 = 0; //Set timer1 to 0   
    //On set le moteur a mode Phase Correct 8Bit et on set le outputMode a set to High when reach top.
    TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM10);  
    TCCR1B = _BV(WGM10); //1/8 Prescaler
    TCCR1C = 0; 
}

/*
@Brief: Cette methode nous permet de changer la valeur de pourcentage de
duty_cycle donnee par l'utilisateur. 
Nous pouvons donc diminiuer la vitesse ou augmenter la vitesse des roues.
@Param: deux valeur de 8bit (uint_8t) pour le pourcentage de dutyCycle de 
la gauche et droite respectivement.
@Return: void
*/
void Moteur::changeSpeed(uint8_t left, uint8_t right){
    startEngine();
    dutyCycleLeft_ = calculerPourcentage(left);
    dutyCycleRight_ = calculerPourcentage(right);
    OCR1A = dutyCycleLeft_;
    OCR1B = dutyCycleRight_;
}
