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
#define AVGSPEED 70
#define HIGHSPEED 100
#define LOWSPEED 60
#define NOSPEED 0
////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
// UART uart;
Del del;
Moteur moteur;
Piezo piezo;
enum CurrentSection {FollowLine, Couloir, Mur, Coupures, Boucles};
CurrentSection current = FollowLine;
/*static void banner(LCM&, char*, uint16_t);
static void wave(LCM&, uint16_t, uint16_t);*/
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1 = false;
bool C2 = false; 
bool C3 = false; 
bool C4 = false;
bool C5 = false;
///////////////////////////////////////////////////////////////
//Definition des fonctions utilisees dans se fichier
void detect();
void followLine();
void dontFollowLine();
void leMur();
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
int main(){
    DDRA = 0x00; //mode entrée
    DDRB = 0xFF; //mode sortie
    DDRD = 0xFF;
    DDRC = 0xFF;
    moteur.startEngine();
    while(true){
        switch(current){

            case FollowLine:
                detect();
                followLine();   
            break;

            case Couloir:
                detect();
                dontFollowLine();
            break;

            case Mur:  
                detect();
                //leMur();
            break;

            case Coupures:
                // TODO
            break;

            case Boucles:
                // TODO
            break;
        }
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
    if(C1==true)
        moteur.changeSpeed(NOSPEED,AVGSPEED);
    else if(C2==true)
        moteur.changeSpeed(NOSPEED,LOWSPEED);
    else if(C3==true && C5==false)
        moteur.changeSpeed(AVGSPEED,AVGSPEED);
    else if(C4==true)
        moteur.changeSpeed(LOWSPEED,NOSPEED);
    else if(C5==true && C3==false)
        moteur.changeSpeed(AVGSPEED,NOSPEED);
    else if(C5==true && C3 == true){  // a gauche
        moteur.stopEngine(); 
        /*_delay_ms(3000); 
        moteur.turnLeft(70); 
        current= FollowLine; */
    }/*
    else if(C3==true  && C5 == true ){  // a droite
        moteur.stopEngine(); 
        _delay_ms(1); 
        moteur.startEngine(); 
        current= FollowLine; 
    }*/
    else
        moteur.changeSpeed(NOSPEED,NOSPEED);
}

void dontFollowLine(){
    if(C1==false)
        moteur.changeSpeed(AVGSPEED,NOSPEED);
    else if(C2==false)
        moteur.changeSpeed(LOWSPEED,NOSPEED);
    else if(C3==false)
        moteur.changeSpeed(NOSPEED,NOSPEED);
    else if(C4==false)
        moteur.changeSpeed(NOSPEED,LOWSPEED);
    else if(C5==false)
        moteur.changeSpeed(NOSPEED,AVGSPEED);
    else if(C1==true && C3==true && C5==true){
        moteur.stopEngine();
        piezo.beep();
        piezo.stop();
        _delay_ms(ONE_SECOND);
        current = Mur;
    }
    else
        moteur.changeSpeed(HIGHSPEED,HIGHSPEED);
}