#ifndef ROBOT_H
#define ROBOT_H

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "memoire_24.h"
#include "del.h"
#include "enums.h"
#include "moteur.h"
#include "piezo.h"
#include "can.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include "bouton.h"

#define CAPTEUR1 0b00000001
#define CAPTEUR2 0b00000010
#define CAPTEUR3 0b00000100
#define CAPTEUR4 0b00001000
#define CAPTEUR5 0b00010000

#define NOSPEED 0
#define LOWSPEED 30
#define SUBSPEED 40
#define AVGSPEED 50
#define HIGHSPEED 60
#define ULTRASPEED 70
#define MAXSPEED 100

#define TWO_SECOND 2000
#define ONE_AND_HALF_SECOND 1500
#define ONE_SECOND 1000
#define HALF_SECOND 500
#define DEBOUNCE_DELAY 200

class Robot {
    public:
        //Default
        Robot();
        ~Robot();

        //Detect Capteurs
        void detect();

        //Methodes necessaire pour les sections du parcours
        void initEndSequence();
        void followLine();
        void turnSequence(const char direction);
        void sonarSendPulse();
        void sonarReadOutput();
        void sonarDetect();
        void stop();
        char boucles_ctr_char_converter(const uint8_t num);
        void bouclesFL();
        void basicTurnSequence(const char direction);

        //Le parcours
        void avancerJusquaCouloir();
        void couloir();
        void avancerCouloirAMur();
        void mur();
        void avancerMurABoucles();
        void boucles();
        void avancerBouclesACoupure();
        void avancerJusquaProchaineCoupure();
        void coupure();
        void avancerCoupureACouloir();


        Bouton btn; 
        UART uart;
        Del del;
        Moteur moteur;
        Piezo piezo;
        
        bool whiteBtnClick;
        volatile uint8_t distance;

        bool C1;
        bool C2; 
        bool C3; 
        bool C4;
        bool C5;
};
#endif