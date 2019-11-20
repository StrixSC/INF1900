/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
Description: 
Fichiers utilisés: 
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
<<<<<<< HEAD
#define AVGSPEED 60
#define HIGHSPEED 80
#define LOWSPEED 50
=======
#define AVGSPEED 70
#define HIGHSPEED 100
#define LOWSPEED 60
>>>>>>> b6e07495b3a0d1a12558e1806a931abae2440740
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
<<<<<<< HEAD
=======
/*static void banner(LCM&, char*, uint16_t);
static void wave(LCM&, uint16_t, uint16_t);*/
>>>>>>> b6e07495b3a0d1a12558e1806a931abae2440740
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1 = false;
bool C2 = false; 
bool C3 = false; 
bool C4 = false;
bool C5 = false;
bool turnLeft = false;
bool turnRight = false;
///////////////////////////////////////////////////////////////
//Definition des fonctions utilisees dans se fichier
void detect();
void followLine();
void dontFollowLine();
void leMur();
void stopSequence();
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
        switch(current){
            case FollowLine:
                if(turnRight){
                    stopSequence();
                        del.vert();
                        moteur.turnRight(AVGSPEED);
                        _delay_ms(1000);
                        del.eteindre();
                    turnRight = false;
                }
                else if(turnLeft){
                    stopSequence();
                    while(!C3){
                        del.rouge();
                        moteur.turnLeft(AVGSPEED);
                        _delay_ms(1000);
                        del.eteindre();

                    }
                }
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
                detect();
            break;

            case Boucles:
                detect();
            break;
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
    //Turn Sequence TEST. Do not delete please -Nawras
    // while(current == FollowLine){
    //     detect();
    //     if(C1 && C2 && C3){
    //         turnLeft = true;
    //         break;
    //     }
    //     else if(C3 && C4 && C5){
    //         turnRight = true;
    //         break;
    //     }
    //     if(C1==true)
    //         moteur.changeSpeed(NOSPEED,AVGSPEED);
    //     else if(C2==true)
    //         moteur.changeSpeed(NOSPEED,LOWSPEED);
    //     else if(C3==true)
    //         moteur.changeSpeed(AVGSPEED,AVGSPEED);
    //     else if(C4==true)
    //         moteur.changeSpeed(LOWSPEED,NOSPEED);
    //     else if(C5==true)
    //         moteur.changeSpeed(AVGSPEED,NOSPEED);
    //     else
    //         moteur.changeSpeed(NOSPEED,NOSPEED);
    // }
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
        _delay_ms(ONE_SECOND); 
        moteur.turnLeft(70); 
        _delay_ms(ONE_SECOND);
        current= FollowLine;
    }
    else if(C3==true  && C5 == true ){  // a droite
        moteur.stopEngine(); 
        _delay_ms(ONE_SECOND); 
        moteur.turnRight(70); 
        _delay_ms(ONE_SECOND);
        current= FollowLine; 
    }
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

void stopSequence(){
    moteur.stopEngine();
    _delay_ms(ONE_SECOND);
    moteur.changeSpeed(LOWSPEED, LOWSPEED);
    _delay_ms(ONE_SECOND);
}

void turnSequence(){
    if(turnRight){
        stopSequence();
        while(!C3){
            moteur.turnRight(AVGSPEED);
        }
        turnRight = false;
    }
    else if(turnLeft){
        stopSequence();
        while(!C3){
            moteur.turnLeft(AVGSPEED);
        }
        turnLeft = false;
    }
}