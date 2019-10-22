#include "del.h"
/*
@Brief: permet dallumer la del soit en rouge,vert ou ambre et la fermer
@Param: port p 
@Return: void (allume la del ou la ferme)
*/
Del::Del() {}

void Del::allumerDEL(Port p){
    switch (p){
        case A: PORTA = 0b01;
        break;

        case B: PORTB = 0b01;
        break;

        case C: PORTC = 0b01;
        break;

        case D: PORTD = 0b01;
        break;
    }
}


void Del::ambre(Port p){

    switch(p){
        case A: 
            PORTA = 0b01;
            _delay_ms(10);
            PORTA = 0b10;
            _delay_ms(10);
        break;

        case B:
            PORTB = 0b01;
            _delay_ms(10);
            PORTB = 0b10;
            _delay_ms(10);
        break;

        case C:
            PORTB = 0b01;
            _delay_ms(10);
            PORTB = 0b10;
            _delay_ms(10);
        break;
            
        case D:
            PORTB = 0b01;
            _delay_ms(10);
            PORTB = 0b10;
            _delay_ms(10);
        break;
    }
}

void Del::eteindre(Port p){
    switch(p){
        case A: PORTA = 0b00;
        break;

        case B: PORTB = 0b00;
        break;

        case C: PORTC = 0b00;
        break;

        case D: PORTD = 0b00;
        break;
    }
}