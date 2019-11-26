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
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "bouton.h"

// #include "LightSensor.h"
#define CAPTEUR1 0b00000001
#define CAPTEUR2 0b00000010
#define CAPTEUR3 0b00000100
#define CAPTEUR4 0b00001000
#define CAPTEUR5 0b00010000
#define DEMO_DDR	DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT	PORTC // Port AVR occup� par l'afficheur

#define AVGSPEED 50
#define LOWERAVGSPEED 40
#define NOSPEED 0
#define LOWSPEED 30
#define HIGHSPEED 60
#define HIGHERSPEED 90
#define MAXSPEED 100
#define ONE_SECOND 1000
#define HALF_SECOND 500

////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
// Memoire24CXXX mem;
LCM disp(&DEMO_DDR, &DEMO_PORT);
Bouton btn; 
UART uart;
Del del;
Moteur moteur;
Piezo piezo;

enum CurrentSection {Test, FollowLine, 
                    Couloir, CouloirAMur, 
                    Mur, MurABoucles, 
                    Boucles, BouclesACoupures, 
                    Coupures, CoupuresACouloir};
CurrentSection current;
///////////////////////////////////////////////////////////////
//Variable boolean pour le stade de nos capteurs
bool C1;
bool C2; 
bool C3; 
bool C4;
bool C5;

bool couloirAMurBool = false;
bool suivreLigne = true;
//Variables pour la section du Mur
volatile uint8_t distance;

//Variables pour les turns Left && Right
bool turnLeft = false;
bool turnRight = false;
uint8_t rightTurnCounter = 4;
uint8_t leftTurnCounter = 4;

//Variables pour la section des boucles:
bool bigLoop = false;
bool smallLoop = false;
bool loopFL = false;
uint8_t loopSequenceCounter = 0;
uint8_t endingLoopSequenceCounter = 0;


///////////////////////////////////////////////////////////////
//Definition des fonctions
void static inline w(void) {
	cp_wait_ms(2000);
}
static void banner(LCM&, char*, uint16_t);
static void wave(LCM&, uint16_t, uint16_t);
void seanceInit();
void detect();
void followLine();
void dontFollowLine();
void sonarReadOutput();
void sonarSendPulse();
void sonarDetect();
void stopSequence();
void turnSequence(const char direction);
void smallLoopSequence();
void bigLoopSequence();
void loopFollowLine();
void endingLoopSequence();
void coupure();
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
    DDRB = 0xFF; 
    DDRD = 0xF0; 
    moteur.startEngine();

    current = Coupures;                 
    disp.clear();
    while(true){
        switch(current){
            case Test:
                if(btn.click()){
                    del.vert();
                }
                else{
                    del.eteindre();
                }
            break;

            case FollowLine:
                //Turn sequence test. Do not delete please - Nawras
                if(turnRight){
                    turnSequence('r');
                }
                else if(turnLeft){
                    turnSequence('l');
                }
                detect();
                followLine();
            break;

            case Couloir:
                detect();
                dontFollowLine();
            break;

            case CouloirAMur:
                detect();
                if(C1 && C2 && C3){
                    stopSequence();
                    turnSequence('l');
                    couloirAMurBool = true;
                }
                else if((!C1 && !C2 && !C3 && !C4 && !C5) && couloirAMurBool){
                    current = Mur;
                }
                else{
                    followLine();
                }
            break;

            case Mur:  
                if(C1 || C2 || C3 || C4 || C5){
                    current = MurABoucles;
                }
                else{
                    sonarDetect();
                }
            break;

            case MurABoucles:
                detect();
            break;

            case Boucles:
                loopFL = true;
                while(loopFL){
                    detect();
                    loopFollowLine();
                    if(loopSequenceCounter == 3){
                        loopSequenceCounter = 0;
                        loopFL = false;
                        bigLoop = true;
                    }
                }
                stopSequence();
                _delay_ms(100);
                turnSequence('l');
                while(bigLoop){
                    detect();
                    bigLoopSequence();
                }
                stopSequence();
                turnSequence('l');
                while(smallLoop){
                    detect();
                    smallLoopSequence();
                }
            break;

            case BouclesACoupures:
                detect();
            break;

            case Coupures:
                detect();
                coupure();

            break;

            case CoupuresACouloir:
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
    if(C1 && C2 && C3 || (C1 && C2 && C3 && C4)){                     
        turnLeft = true;
    }
    else if(C3 && C4 && C5 || (C2 && C3 && C4 && C5)){                 
        turnRight = true;
    }
    else if(C3 || (C2 && C3 && C4)){         
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C2 || (C1 && C2)){              
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C4 || (C4 && C5)){              
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
    else if(C1){                            
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C5){                            
        moteur.changeSpeed(AVGSPEED, NOSPEED);  
    }
}

void dontFollowLine(){
    detect();
    if(C3 || (C2 && C3) || (C3 && C4) || (C2 && C3 && C4)){
        moteur.changeSpeed(NOSPEED, NOSPEED);
        _delay_ms(ONE_SECOND);
        current=CouloirAMur;
    }
    else if(C5 || (C4 && C5)){                           
        moteur.changeSpeed(NOSPEED, MAXSPEED);
        _delay_ms(100);
    }
    else if(C1 || (C1 && C2)){                           
        moteur.changeSpeed(MAXSPEED, NOSPEED);
        _delay_ms(50);
    }
    else
        moteur.changeSpeed(HIGHSPEED, LOWERAVGSPEED);
}

void stopSequence(){
    moteur.changeSpeed(AVGSPEED, AVGSPEED);
    _delay_ms(HALF_SECOND);
    moteur.changeSpeed(NOSPEED,NOSPEED);
    _delay_ms(ONE_SECOND);
}

void turnSequence(const char direction){
    if(direction == 'r'){
        stopSequence();
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C3 || C4 || C5){
                del.rouge();
                turnBool = false;
            }
            else{
                del.vert();
                moteur.turnRight(LOWERAVGSPEED);
            }
        }
        turnRight = false;
        // rightTurnCounter--;
    }
    else if(direction == 'l'){
        stopSequence();
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C1 || C2 || C3){
                del.rouge();
                turnBool = false;
            }
            else{
                moteur.turnLeft(AVGSPEED);
            }
        }
        turnLeft = false;
    }
    else {              //Si le input de l'utilisateur n'est pas L ou R. 
        return;
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
    if(distance >=12  && distance <= 18){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(distance < 12){
        moteur.changeSpeed(LOWERAVGSPEED,0);
    }
    else if(distance > 18){
        moteur.changeSpeed(0, LOWERAVGSPEED);
    }
}

void loopFollowLine(){
    del.rouge();
    if((C1 && C2 && C3) || (C2 && C3) || (C1 && C2 && C3 && C4) || (C2 && C3 && C4)){
        _delay_ms(300); //Necessary delay for debounce
        if((C1 && C2 && C3) || (C2 && C3) || (C1 && C2 && C3 && C4) || (C2 && C3 && C4))
            loopSequenceCounter++;
    }
    else if(C3 || (C2 && C3 && C4)){                            
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C4){
        moteur.changeSpeed(LOWERAVGSPEED, NOSPEED);
    }
    else if((C4 && C5) || C5){        
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
    else if(C1 || C2){                     
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(!C1 && !C2 && !C3 && !C4 && !C5){
        moteur.changeSpeed(NOSPEED, NOSPEED);
    }
}

void bigLoopSequence(){
    del.vert();
    if(C1 && C2 && C3 && C4 && C5 || (C2 && C2 && C3 && C4)){
        bigLoop = false;
        smallLoop = true;
    }
    else if((C1 && C2 && C3) || (C2 && C3) || (C4 && C3 && C2 && C1)){
        stopSequence();
        turnSequence('l');
    }
    else if(C1){
        moteur.changeSpeed(NOSPEED, HIGHSPEED);
    }
    else if(C2){
        moteur.changeSpeed(NOSPEED, LOWERAVGSPEED);
    }
    else if(C4){
        moteur.changeSpeed(LOWERAVGSPEED, NOSPEED);
    }
    else if(C5){
        moteur.changeSpeed(HIGHSPEED, NOSPEED);
    }
    else if(C3 || C2 && C3 && C4){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(!C1 && !C2 && !C3 && !C4 && !C5){
        moteur.changeSpeed(NOSPEED, NOSPEED);
    }
}

void smallLoopSequence(){
    del.ambre();
    if((C1 && C2 && C3 && C4 && C5) || (C5 && C2 && C3 && C4)){
        smallLoop = false;
    }
    else if(C1 && C2 && C3 || C2 && C3){
        stopSequence();
        turnSequence('l');
    }
    else if(C3 || (C2 && C3 && C4)){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C4 || C4 && C5){
        moteur.changeSpeed(LOWERAVGSPEED, NOSPEED);
    }
    else if(C2 || C2 && C1){
        moteur.changeSpeed(NOSPEED, LOWERAVGSPEED);
    }
    else if(C1){
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C5){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
    else if(!C1 && !C2 && !C3 && !C4 && !C5){
        moteur.changeSpeed(NOSPEED, NOSPEED);
    }
}

void endingLoopSequence(){
    if((C1 && C2 && C3) || (C2 && C3) || (C1 && C2 && C3 && C4) || (C2 && C3 && C4)){
        _delay_ms(300);     //Necessary delay for debounce
        if((C1 && C2 && C3) || (C2 && C3) || (C1 && C2 && C3 && C4) || (C2 && C3 && C4))
            endingLoopSequenceCounter++;
    }
    if(C3 || (C2 && C3 && C4)){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C5){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
    else if(C4 || (C4 && C5)){
        moteur.changeSpeed(LOWERAVGSPEED, NOSPEED);
    }
    else if(C1){
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C2 || (C2 && C5)){
        moteur.changeSpeed(NOSPEED, LOWERAVGSPEED);
    }
    else if(!C1 && !C2 && !C3 && !C4 && !C5){
        moteur.changeSpeed(NOSPEED, NOSPEED);
    }
}

///////////////////////////////////////////////////////////////////////////////
//Fonctions pour faire fonctionner le LCD Display.
//Nous n'avons pas ecrite ses fonctions, elles sont copiees avec liscence de redistribution.
/**
 * Fonction pour d�mo - banni�re de texte.
 * @param lcm		R�f�rence vers un LCM
 * @param text		Texte � afficher (jeu de caract�res de l'afficheur)
 * @param ms_interval	Intervalle en millisecondes entre chaque d�callage
 */
static void banner(LCM& lcm, char* text, uint16_t ms_interval) {
	uint8_t start = 0, stop = 1, index = 0;
	uint16_t msg_len = cp_strlen(text);
	char t_char = 0;
	
	lcm.clear();
	cp_wait_ms(ms_interval);
	
	while (stop <= msg_len && start < msg_len - 1) {
		if (stop <= LCM_FW_HALF_CH) {
			index = LCM_FW_HALF_CH - stop;
		} else {
           	++start;
			index = 0;
		}
		
		t_char = text[stop];
		text[stop] = 0;
		lcm.write(text + start, index, stop == msg_len);
		text[stop] = t_char;

		if (stop != msg_len) {
			++stop;
		}
		cp_wait_ms(ms_interval);
	}
	
	lcm.clear();
	cp_wait_ms(ms_interval);
}
/**
 * Fonction pour d�mo - vague.
 *
 * @param lcm		R�f�rence vers un LCM
 * @param rep		Nombre de r�p�titions
 * @param ms_interval	Intervalle en millisecondes entre chaque mouvement
 */
static void wave(LCM& lcm, uint16_t rep, uint16_t ms_interval) {
	uint8_t t_per [8];
	
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			if (j == i) {
				t_per[j] = 0xff;
			} else {
				t_per[j] = 0;
			}
		}
		lcm.build_cc(i, t_per);
	}
	
	char custom_l1 [] = "\x20\x07\x05\x04\x03\x02\x02\x01\x01\x01\x02\x02"
		"\x03\x04\x05\x07\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x07\x05\x04\x03\x02\x02\x01\x01\x01"
		"\x02\x02\x03\x04\x05\x07";
	char custom_l2 [] = "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x08\x02\x03\x04\x05\x05\x06\x06\x06\x05"
		"\x05\x04\x03\x02\x08\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20";

	char t_char;
	
	lcm.clear();
	
	for (uint16_t k = 0; k < rep; ++k) {
		for (uint8_t i = 0; i < LCM_FW_TOT_CH; ++i) {
			t_char = custom_l1[i + 16];
			custom_l1[i + LCM_FW_HALF_CH] = 0;
			
			lcm.write(custom_l1 + i, 0);
			
			custom_l1[i + LCM_FW_HALF_CH] = t_char;
			t_char = custom_l2[i + LCM_FW_HALF_CH];
			custom_l2[i + LCM_FW_HALF_CH] = 0;
			
			lcm.write(custom_l2 + i, 16);
			
			custom_l2[i + LCM_FW_HALF_CH] = t_char;
			
			cp_wait_ms(ms_interval);
		}
	}
	
	lcm.clear();
}

void coupure(){
    if(C1==true || C2==true || C3==true || C4==true || C5==true){
        followLine();
    }
    /*else if(!C2 && !C3 && !C4){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }*/
    else if(!C1 && !C2 && !C3 && !C4 && !C5){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
}
    
///////////////////////////////////////////////////////////////////////////////