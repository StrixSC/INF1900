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

#define AVGSPEED 60
#define NOSPEED 0
#define LOWSPEED 30
#define HIGHSPEED 60
#define ONE_SECOND 1000
#define HALF_SECOND 500

////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
UART uart;
Del del;
Moteur moteur;
Piezo piezo;
enum CurrentSection {FollowLine, Couloir, Mur, Coupures, Boucles};
CurrentSection current = FollowLine;
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1;
bool C2; 
bool C3; 
bool C4;
bool C5;
volatile uint8_t distance;
bool turnLeft = false;
bool turnRight = false;
///////////////////////////////////////////////////////////////
//Definition des fonctions
void seanceInit();
void detect();
void followLine();
void dontFollowLine();
void sonarReadOutput();
void sonarSendPulse();
void sonarDetect();
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
    DDRA |= _BV(PA6);
    DDRB = 0xFF; //mode sortie
    DDRD = 0xFF;
    moteur.startEngine();
    current = Mur;
    while(true){
        switch(current){
            case FollowLine:
                //Turn sequence test. Do not delete please - Nawras
                // if(turnRight){
                //     stopSequence();
                //         del.vert();
                //         moteur.turnRight(AVGSPEED);
                //         _delay_ms(1000);
                //         del.eteindre();
                //     turnRight = false;
                // }
                // else if(turnLeft){
                //     stopSequence();
                //     while(!C3){
                //         del.rouge();
                //         moteur.turnLeft(AVGSPEED);
                //         _delay_ms(1000);
                //         del.eteindre();
                //     }
                // }
                detect();
                followLine();
            break;

            case Couloir:
                detect();
                dontFollowLine();
            break;

            case Mur:  
                sonarDetect();
            break;
            case Coupures:
                detect();
            break;
            case Boucles:
                detect();
            break;
        }
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
    if(C1==true && C2==false){
        moteur.changeSpeed(0,50);
    }
    else if(C1==false && C2==true){
        moteur.changeSpeed(0,40);
    }
    else if(C1==false && C2==false && C3==true){
        moteur.changeSpeed(50,50);
    }
    else if(C4==true){
        moteur.changeSpeed(40,0);
    }
    else if(C5==true){
        moteur.changeSpeed(50,0);
    }
    else if(C1==true && C2==true && C3==true || C1==true && C2==true && C3==true && C4==true){
        moteur.changeSpeed(0,60);
    }
    else if(C3==true && C4==true && C5==true || C2==true && C3==true && C4==true && C5==true){
        moteur.changeSpeed(60,0);
    }
    /*
    else if(C1==false && C2==false && C3==false && C4==false && C5==false){
        moteur.changeSpeed(0,0);
    }*/
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


void sonarSendPulse(){
    PORTA |= _BV(PA6);   
    _delay_us(15);
    PORTA &= ~_BV(PA6);
}

void sonarReadOutput(){
    while(!(PINA & _BV(PA7))){}
    uint16_t counter = 0;
    while((PINA & _BV(PA7))){
        _delay_us(0.5);
        counter++;
    };
    distance = (counter/58);
}

void sonarDetect(){
    moteur.startEngine();
    sonarSendPulse();
    sonarReadOutput();
    _delay_ms(50);
    uart.transmissionUART(distance);
    if(distance >=10  && distance <= 20){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(distance < 10){
        moteur.changeSpeed(AVGSPEED,0);
    }
    else if(distance > 20){
        moteur.changeSpeed(0, AVGSPEED);
    }
    
    // else if(distance > 14){
    //     moteur.changeSpeed(AVGSPEED, NOSPEED);
    // }
    // else if(distance < 16){  
    //     moteur.changeSpeed(AVGSPEED, NOSPEED);
    // }

}