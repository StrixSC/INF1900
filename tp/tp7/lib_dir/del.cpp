#include "del.h"

/*
@Brief: Constructeur 
@Param: aucun
@Return: void 
*/

Del::Del() {}

/*
rouge = ob01
vert = ob10
ferme = ob00
*/


/*
@Brief: permet dallumer la del soit en rouge,vert 
@Param: port p 
@Return: void 
*/
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

/*
@Brief: permet dallumer la del soit en couleur ambre
@Param: port p 
@Return: void 
*/
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

/*
@Brief: permet d'eteindre la del 
@Param: port p 
@Return: void (
*/

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