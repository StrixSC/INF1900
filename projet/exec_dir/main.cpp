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
// #include "LightSensor.h"

#define CAPTEUR1 0b00000001
#define CAPTEUR2 0b00000010
#define CAPTEUR3 0b00000100
#define CAPTEUR4 0b00001000
#define CAPTEUR5 0b00010000

////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
// UART uart;
Del del;
Moteur moteur;
// Piezo piezo;
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1 = false;
bool C2 = false; 
bool C3 = false; 
bool C4 = false;
bool C5 = false;
///////////////////////////////////////////////////////////////
//Definition des fonctions
void seanceInit();
void detect();
void followLine();
///////////////////////////////////////////////////////
///////////                                 ///////////
///////////               MAIN              ///////////
///////////                                 ///////////
///////////////////////////////////////////////////////
/*
@Brief: 
@param: void;
@return: int;
*/
int main(){
    DDRA = 0x00; //mode entrée
    DDRB = 0xFF; //mode sortie
    DDRD = 0xFF;
    moteur.startEngine();

    while(true){
        detect();
        followLine();
    }
}

/*
@Brief: Cette fonction ne sert qu'a faire clignoter la del en rouge 2 fois et 
en vert deux fois, pour signifier le debut de la routine.
@param: void;
@return void;
*/
void seanceInit(){
   for(uint8_t i =0; i<2; i++){
       del.rouge();
       _delay_ms(500);
       del.eteindre();
       _delay_ms(500);
   }

   for(uint8_t i =0; i<2; i++){
       del.vert();
       _delay_ms(500);
       del.eteindre();
       _delay_ms(500);
   }

}

/*
@Brief: Modifie le stade des variables de capteurs dependamment du signal que le 
detecteur de line Cytron envoie au PINS A.
@param: void;
@return void;
*/
void detect(){
    if(PINA & CAPTEUR1)
        C1 = true;
    else if(!(PINA & CAPTEUR1))
        C1 = false;
    if(PINA & CAPTEUR2)
        C2 = true;
    else if(!(PINA & CAPTEUR2))
        C2 = false;
        
    if(PINA & CAPTEUR3)
        C3 = true;
    else if(!(PINA & CAPTEUR3))
        C3 = false;
    
    if(PINA & CAPTEUR4)
        C4 = true;
    else if(!(PINA & CAPTEUR4))
        C4 = false;
    
    if(PINA & CAPTEUR5)
        C5 = true;
    else if(!(PINA & CAPTEUR5))
        C5 = false;
}

void followLine(){
    if(C3){                              //0 0 1 0 0 
        moteur.changeSpeed(50,50);
    }      
    else if(C2 && C3)                   //0 1 1 0 0 
        moteur.changeSpeed(40,60); 
    else if(C3 && C4)                   //0 0 0 1 1
        moteur.changeSpeed(60,40);      
    else if(C3 && C4 && C5)             //0 0 1 1 1 -> rightTurn
        moteur.turnRight(50);
    else if(C1 && C2 && C3)             //1 1 1 0 0 -> leftTurn
        moteur.turnLeft(50);
    else
        moteur.stopEngine();
}