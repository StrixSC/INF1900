#include "minuterie.h"

Minuterie::Minuterie(){
    init();
}

Minuterie::~Minuterie(){
    //
}

void Minuterie::start(uint8_t time){
    OCR0A = time;
}

void Minuterie::stop(){
    OCR0A = 0;  //On remet le TOP value a 0;
    TCNT0 = 0;  //On remet le timer a 0;
    TCCR0B = 0; //On arrete le timer.
}

void Minuterie::init(){
    TCNT0 = 0; //Set timer0 to 0   
    //On set le moteur a mode Normal CTC mode. Out set to high on compare match.
    TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(WGM10);
    TCCR0B = _BV(CS00) | _BV(CS02); //Prescaler de 1024
}