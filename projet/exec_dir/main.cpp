/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
**************************************************************************************************************************/
#include "Robot.h"

#define END 0
#define COULOIR 1
#define MUR 2
#define BOUCLES 3 
#define COUPURES 4

#define DEMO_DDR DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT PORTC // Port AVR occup� par l'afficheur
Robot robot;
LCM disp(&DEMO_DDR, &DEMO_PORT);
uint8_t currentSection = 0;
uint8_t initSection = 0;
volatile uint16_t clickCounter = 0;
volatile bool lcd = true;
volatile bool init = false;
void static inline w(void) {
	cp_wait_ms(2000);
}
        
void initialisation(void) {
  cli ();
  EIMSK |= (1 << INT0) | (1 << INT1);

  EICRA |= (1 << ISC00) | (1 << ISC11); 
  sei ();
}

//Interrupt service routine pour le bouton poussoir
ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        init = true;
    }
    else{
        init = false;
    }
}

//Interrupt service routine pour le bouton blanc
ISR(INT1_vect){
    _delay_ms(30);
    if(!(PIND & (1 << PD3))){
        lcd = true;
        clickCounter++;
    }
    else{
        init = false;
    }
    EIFR |= (1 << INTF1);
}

//Cette methode permet de selectionner la section du parcours par lequel le robot debutera
void setInitSection(const uint8_t section){
    switch(section)
    {
    case COULOIR: 
            initSection = 1;
        break;
    case MUR: // affiche « le mur »
            initSection = 2;
        break;
    case BOUCLES: // affiche « les deux boucles »
            initSection = 3;
        break;
    case COUPURES: // affiche « les coupures »
            initSection = 4;
        break;
    case END: // affiche « fin »
            initSection = 0;
        break;
    }
}

//Set methode permet d'afficher la section courrante du robot
void showCurrentSection(const uint8_t section){
    switch (section)
    {
        case END: 
            disp = "SHUTDOWN";
            break;
        case COULOIR: 
            disp = "Le Couloir";
            break;
        case MUR: 
            disp = "Le Mur";
            break;
        case BOUCLES: 
            disp = "Les Deux Boucles";
            break;
        case COUPURES:
            disp = "Les Coupures";
            break;
    }
}

//Cette methode permet de changer de section d'une a l'autre. Si la section a laquelle on change est la section
//initiale, le robot s'arrete et le parcours se termine.
void sectionTransition(){
    switch(currentSection){
        case END: //
        break;
        
        case BOUCLES:
            if(initSection != COUPURES)
                currentSection = COUPURES;
            else 
                currentSection = END;
        break;

        case COUPURES:
            if(initSection != COULOIR)
                currentSection = COULOIR; 
            else
                currentSection = END;
        break;

        case COULOIR:
            if(initSection != MUR)
                currentSection = MUR; 
            else
                currentSection = END;
        break;

        case MUR:
            if(initSection != BOUCLES)
                currentSection = BOUCLES; 
            else
                currentSection = END;
        break;
    }
}

///////////////////////////////////////////////////////
///////////                                 ///////////
///////////               MAIN              ///////////
///////////                                 ///////////
///////////////////////////////////////////////////////
int main(){
    initialisation();
    robot.stop();
    while (!init)
    {   
        if(lcd)
        {    
            switch (clickCounter % 5)  //Modulo 5 pour ne pas avoir a reset le counter si on click plus que 4 fois.
            {
                case COULOIR:
                    setInitSection(COULOIR);
                break;
                case MUR:
                    setInitSection(MUR);
                break;
                case BOUCLES:
                    setInitSection(BOUCLES);
                break;
                case COUPURES:
                    setInitSection(COUPURES);
                break;
            }
            showCurrentSection(initSection);
            lcd = false;
        }
    }

    currentSection = initSection;
    while(currentSection != END){
        //On passe a travers chaque section jusqu'a ce que le parcours n'arrive pas a la fin.
        switch(currentSection){
            case COUPURES:
                showCurrentSection(currentSection);
                robot.boost();
                robot.detect();
                robot.coupure();
                robot.detect();
                robot.avancerCoupureACouloir();
                robot.boost();
                sectionTransition();
            break;

            case COULOIR:
                showCurrentSection(currentSection);
                robot.boost();
                robot.detect();
                robot.avancerJusquaCouloir();
                robot.detect();
                robot.couloir();
                robot.detect();
                robot.avancerCouloirAMur();
                robot.boost();
                sectionTransition();
            break;

            case MUR:
                showCurrentSection(currentSection);
                robot.boost();
                robot.detect();
                robot.preMur();
                robot.detect();
                robot.mur();
                robot.detect();
                robot.avancerMurABoucles();
                robot.boost();
                sectionTransition();
            break;

            case BOUCLES:
                showCurrentSection(currentSection);
                robot.boost();
                robot.detect();
                robot.boucles();
                robot.detect();
                robot.avancerBouclesACoupure();  
                robot.boost();   
                sectionTransition();
            break;

            case END:
            break;
        }
    }
    //On stop le robot et on termine le parcuors.
    robot.stop();
    robot.initEndSequence();

}
