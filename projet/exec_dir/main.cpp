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
#define AVGSPEED 42
#define HIGHSPEED 50
#define LOWSPEED 40

////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
// UART uart;
Del del;
Moteur moteur;
Piezo piezo;
enum CurrentSection {FollowLine, Couloir, Mur, Coupures, Boucles};
CurrentSection current = FollowLine;
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
    moteur.startEngine();
    while(true){
        detect();
        followLine();
        // switch(current){
        //     case FollowLine:
        //         detect();
        //         followLine();   
        //     break;

        //     case Couloir:
        //         detect();
        //         dontFollowLine();
        //     break;

        //     case Mur:  
        //         // TODO
        //     break;

        //     case Coupures:
        //         // TODO
        //     break;

        //     case Boucles:
        //         // TODO
        //     break;
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

}

void dontFollowLine(){
    if(C1 && !C2 && !C3 && !C4 && C5){              //1 0 0 0 1
        moteur.changeSpeed(AVGSPEED,AVGSPEED);
    }
    else if(C1 && !C2 && !C3 && !C4 && !C5) {              //1 0 0 0 0
        moteur.changeSpeed(HIGHSPEED,AVGSPEED);                  
    }
    else if(!C1 && !C2 && !C3 && !C4 && C5) {              //0 0 0 0 1
        moteur.changeSpeed(AVGSPEED,HIGHSPEED);                  
    }
    else if((!C1 && C2 && !C3 && !C4 && !C5) 
    || (C1 && C2 && !C3 && !C4 && !C5)){                   //0 1 0 0 0 or 1 1 0 0 0
        moteur.changeSpeed(HIGHSPEED,AVGSPEED);                  
    }
    else if((!C2 && !C3 && C4) 
    || (!C2 && !C3 && C4 && C5)){                   //0 0 0 1 0 or 0 0 0 1 1
        moteur.changeSpeed(AVGSPEED,HIGHSPEED);                  
    }

    else if (C1 && !C2 && C3 && !C4 && C5){
        moteur.stopEngine();
        piezo.beep();
        piezo.stop();
        _delay_ms(ONE_SECOND);
        current = FollowLine;              //Switch le currentStage a Couloir.
    }
}