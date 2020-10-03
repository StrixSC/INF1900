#include "del.h"

/*
@Brief: Constructeur 
@Param: aucun
@Return: void 
*/

Del::Del() {}

/*
@Brief: permet dallumer la del en vert
@Param: port p 
@Return: void 
*/
void Del::vert(){
    PORTB = 0b01;
}


/*
@Brief: permet dallumer la del en rouge
@Param: void
@Return: void 
*/
void Del::rouge(){
    PORTB = 0b10;
}

/*
@Brief: permet dallumer la del soit en couleur ambre
@Param: port p 
@Return: void 
*/
void Del::ambre(){
    vert();
    _delay_ms(10);
    rouge();
    _delay_ms(10);
}

/*
@Brief: permet d'eteindre la del 
@Param: port p 
@Return: void (
*/
void Del::eteindre(){
    PORTB = 0b00;
}