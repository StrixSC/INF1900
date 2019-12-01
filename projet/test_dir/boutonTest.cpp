/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
Description: Ce fichier permet de faire passer au robot différents états pour lui faire faire ce que nous désirons en fonction de ce qu'il lui est passé en paramètres
            grâce aux états du switch case.
Fichiers utilisés: Uart.h, includes.h, memoire_24.h, del.h, enums.h, moteur.h

**************************************************************************************************************************/

#include "UART.h"
#include "includes.h"
#include "memoire_24.h"
#include "del.h"
#include "enums.h"
#include "moteur.h"
#include "piezo.h"
#include "can.h"

#define CAPTEUR1 0b00000001
#define CAPTEUR2 0b00000010
#define CAPTEUR3 0b00000100
#define CAPTEUR4 0b00001000
#define CAPTEUR5 0b00010000
#define ONE_SECOND 1000
#define HALF_SECOND 500
#define AVGSPEED 50
#define HIGHSPEED 80
#define LOWSPEED 40
#define NOSPEED 0
////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
UART uart;
Del del;
Moteur moteur;
Piezo piezo;
uint8_t counter = 0;
uint8_t whiteBtnClick = 0;
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1 = false;
bool C2 = false; 
bool C3 = false; 
bool C4 = false;
bool C5 = false;

uint8_t distance;
uint8_t btnCounter = 0;
///////////////////////////////////////////////////////
///////////                                 ///////////
///////////               MAIN              ///////////
///////////                                 ///////////
///////////////////////////////////////////////////////
/*
@Brief: Fonction main
@param: void;
@return: int;
*/

void initialisation ( void ) {
  cli ();
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00); 
  sei ();
}

ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        _delay_ms(50);
        if(PIND & 0x04){
            btn.setClicked(1);
        }
    }
    else{
        btn.setClicked(0);
    }
}

ISR(INT1_vect){
    _delay_ms(30);
    if(PIND & 0x08){
        _delay_ms(50);
        if(PIND & 0x08){
            whiteBtnClick = 1;
        }
    }
    else {
        whiteBtnClick = 0;
    }
}

int main {
    initialisation();
    DDRB = 0xFF; //mode sortie
    DDRD = 0x00;

    while(true){
        if(btn.getClicked()){
            del.vert();
        }
        else if (btn.getClicked() == 0){
            del.eteindre();
        }
        else if(whiteBtnClick){
            del.vert();
        }
        else if(!whiteBtnClick){
            del.eteindre();
        }     
    }
}
