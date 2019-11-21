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
enum CurrentSection {FollowLine, Couloir, Mur, Coupures, Boucles};
CurrentSection current = FollowLine;
uint8_t counter = 0;
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1 = false;
bool C2 = false; 
bool C3 = false; 
bool C4 = false;
bool C5 = false;

uint8_t distance;
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

void sonarSendPulse(){
    PORTA |= _BV(PA0);   
    _delay_us(15);
    PORTA &= ~_BV(PA0);
}

void sonarReadOutput(){
    while(!(PINA & _BV(PA1))){}
    uint16_t counter = 0;
    while((PINA & _BV(PA1))){
        _delay_us(0.5);
        counter++;
    };
    distance = (counter/58);
}

int main(){
    DDRA |= (1 << PA0);
    DDRB = 0xFF; //mode sortie
    while(true){
        sonarSendPulse();
        sonarReadOutput();
        _delay_ms(50);
        uart.transmissionUART(distance);
        if(distance >= 14 && distance <= 16){
            del.vert();
        }
        else {  
            del.eteindre();
        }
    }
}
