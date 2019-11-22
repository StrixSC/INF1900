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
#define LOWERAVGSPEED 40
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
uint8_t rightTurnCounter = 4;
uint8_t leftTurnCounter = 4;
///////////////////////////////////////////////////////////////
//Definition des fonctions
void seanceInit();
void detect();
void followLine();
void dontFollowLine();
void sonarReadOutput();
void sonarSendPulse();
void sonarDetect();
void stopSequence();
void receiveUartOutputs();
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
    current = FollowLine;
    while(true){
        switch(current){
            case FollowLine:
                //Turn sequence test. Do not delete please - Nawras
                if(turnRight && rightTurnCounter){
                    stopSequence();
                        del.vert();
                        moteur.turnRight(HIGHSPEED);
                        _delay_ms(3000);
                        del.eteindre();
                    turnRight = false;
                }
                else if(turnLeft && leftTurnCounter){
                    stopSequence();
                    while(!C3){
                        del.rouge();
                        moteur.turnLeft(HIGHSPEED);
                        _delay_ms(ONE_SECOND);
                        del.eteindre();
                    }
                    turnLeft = false;
                }
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
    // if((C1 && C2 && C3 && !C4 && !C5) || (C1 && C2 && C3 && C4 && !C5)){                                              //1 1 1 0 0 OU 1 1 1 1 0 
    //     turnLeft = true;
    // }
    // else if((!C1 && !C2 && C3 && C4 && C5) || (!C1 && C2 && C3 && C4 && C5)){       //0 0 1 1 1 OU 0 1 1 1 1
    //     turnRight = true;
    // }
    // else if((C1 && C2 && !C3 && !C4 && !C5) || (!C1 && C2 && C3 && !C4 && !C5)){    //1 1 0 0 0 OU 0 1 1 0 0
    //     moteur.changeSpeed(0,AVGSPEED);
    // }
    // else if(C1 && !C2 && !C3 && !C4 && !C5){                                        //1 0 0 0 0 
    //     moteur.changeSpeed(0,AVGSPEED);
    // }
    // else if(!C1 && C2 && !C3 && !C4 && !C5){                                        //0 1 0 0 0 
    //     moteur.changeSpeed(0,LOWERAVGSPEED);
    // }
    // else if(
    //     (!C1 && !C2 && C3 && !C4 && !C5) ||
    //     (!C1 && C2 && C3 && C4 && !C5)){                        //0 0 1 0 0                       
    //     moteur.changeSpeed(AVGSPEED,AVGSPEED);
    // }
    // else if(!C1 && !C2 && !C3 && C4 && !C5){                        //0 0 0 1 0 
    //     moteur.changeSpeed(LOWERAVGSPEED,0);
    // }
    // else if(!C1 && !C2 && !C3 && !C4 && C5){                        //0 0 0 0 1
    //     moteur.changeSpeed(AVGSPEED,0);
    // }
    // else if(!C1 && !C2 && !C3 && C4 && C5 || (!C1 && !C2 && C3 && C4 && !C5)){ //0 0 0 1 1
    //     moteur.changeSpeed(LOWERAVGSPEED,0);
    // }
    // else {                                                          //0 0 0 0 0
    //     moteur.changeSpeed(NOSPEED, NOSPEED);
    // }
    // /*
    // else if(C1==false && C2==false && C3==false && C4==false && C5==false){
    //     moteur.changeSpeed(0,0);
    // }*/

    
    if(C1 && C2 && C3 || (C1 && C2 && C3 && C4)){                     //1 1 1 X X ou // 1 1 1 1 X
        turnLeft = true;
    }
    else if(C3 && C4 && C5 || (C2 && C3 && C4 && C5)){                //X X 1 1 1 ou // X 1 1 1 1 
        turnRight = true;
    }
    else if(C3 || (C2 && C3 && C4)){        //0 0 1 0 0 OU 0 1 1 1 0 0 
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C2 || (C1 && C2)){              //X 1 X X X OU 1 1 X X X
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C4 || (C4 && C5)){              // X X X 1 X OU X X X 1 1
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
    else if(C1){                            //1 X X X X
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C5){                            //X X X X 1 
        moteur.changeSpeed(AVGSPEED, NOSPEED);  
    }
    else {                                  // X X X X X
        moteur.changeSpeed(NOSPEED, NOSPEED);
    }
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
    moteur.changeSpeed(NOSPEED,NOSPEED);
    _delay_ms(ONE_SECOND);
    moteur.changeSpeed(HIGHSPEED, HIGHSPEED);
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

void receiveUartOutputs(){
    uart.transmissionUART(C1);
    uart.transmissionUART(C2);
    uart.transmissionUART(C3);
    uart.transmissionUART(C4);
    uart.transmissionUART(C5);
}