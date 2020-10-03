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

//Liste des capteurs utilise et de leurs ports specifiques
#define CAPTEUR1 0b00000001
#define CAPTEUR2 0b00000010
#define CAPTEUR3 0b00000100
#define CAPTEUR4 0b00001000
#define CAPTEUR5 0b00010000

//Liste des vitesses utilisee dans le robot pour les moteurs.
#define NOSPEED 0       
#define LOWSPEED 35
#define SUBSPEED 45
#define AVGSPEED 50
#define HIGHSPEED 60
#define ULTRASPEED 70
#define MAXSPEED 100

//Liste des temps de delays utilise
#define TWO_SECOND 2000
#define ONE_AND_HALF_SECOND 1500
#define ONE_SECOND 1000
#define HALF_SECOND 500
#define DEBOUNCE_DELAY 200  //Delay donne lorsqu'on veut s'assurer que les capteurs on bel et bien capte
                            //ce qu'ils voulaient capter.

class Robot {
    public:
        //Default
        Robot();
        ~Robot();

        //Detect Capteurs
        void detect();

        //////////////////////////////////////////////////
        //Methodes necessaire pour les sections du parcours

        //Cette methode ne fait que montrer grace aux dels que les sections sont termines.
        void initEndSequence();

        //Premiere version de followLine, on utilise les capteurs pour que le robot 
        //Ajuster la vitesse de ses roues et suivre une ligne de tape noir.
        void followLine();

        //Deuxieme version de followLine, on utilise celle-ci plus souvent.
        void basicFollowLine();

        //TurnSequence prend un character en param (r ou l) et permet de tourner le robot vers cette dir.
        //On arrete de tourner des que le robot capte le capteur 3, voulant dire quil est arriver a tourner 
        //completement.
        void turnSequence(const char direction);

        //SoanrSendPulse et SonarReadOutput sont les deux methodes utilisee pour faire la section Mur.
        //avec SonarSendPulse, on allume le PA7 et on envoie du courant pour 10ns au sonar. 
        void sonarSendPulse();
        void sonarReadOutput(); //Avec SonarReadOutput, on peut lire le output recu par le PD6 du sonar.
        //En utilisant les deux methodes sonarSendPulse et sonarReadOutput, on peut obtenir la distance
        //entre le robot et le mur.

        void stop();    //Stop permet d'arrter les roues du moteur.         
        void boucleCheck();     //Cette methode permet de verifier si on est rendu a un coin dans la section boucle
        void basicTurnSequence(const char direction);   //La deuxieme methode de turnSequence. On utilise 
        //celle-ci si on veut que le robot ne tourne qu'avec une roue.
        void boucleTurnSequence(const char direction);
        //Cette version de turnSequence est adaptee pour la section Boucle.

        //////////////////////////////////////////////////////////
        //Le parcours
        //Les methodes suivantes sont pas mal evidente. Elle ne font que permettre au robot d'effectuer les 
        //sections du trajet. 
        void avancerJusquaCouloir();
        void couloir();
        void avancerCouloirAMur();
        void preMur();
        void mur();
        void avancerMurABoucles();
        void boucles();
        void avancerBouclesACoupure();
        void avancerJusquaProchaineCoupure();
        void coupure();
        void avancerCoupureACouloir();
        void boost();


        //Nous n'avons pas mit les attributs en prive, pour faciliter l'acces.
        Bouton btn; 
        UART uart;      //le uart n'etait qu'utiliser pour obtenir les valeurs du sonar. Elle n'est plus
                        //utilisee apres.
        Del del;
        Moteur moteur;
        Piezo piezo;
        
        bool whiteBtnClick;         //Notre boolean pour nous permettre de savoir si le boutonBlacn sur le bread
                                    //board est clickee
        volatile uint8_t distance;  //La distance du entre le robot et le mur pour la section Mur.

        //Les booleans pour chaque capteur. Ceux-ci sont changer constamment de true a false et vice versa.
        bool C1;                    
        bool C2; 
        bool C3; 
        bool C4;
        bool C5;
};
#endif