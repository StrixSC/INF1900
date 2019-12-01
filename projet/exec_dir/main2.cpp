/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
Description: Ce fichier permet de faire passer au robot différents états pour lui faire faire ce que nous désirons en fonction de ce qu'il lui est passé en paramètres
            grâce aux états du switch case.
Fichiers utilisés: Uart.h, includes.h, memoire_24.h, del.h, enums.h, moteur.h

**************************************************************************************************************************/
#include "Robot.h"

#define DEMO_DDR DDRC // `Data Direction Register' AVR occup� par l'aff.
#define DEMO_PORT PORTC // Port AVR occup� par l'afficheur
Robot robot;
enum CurrentSection {Couloir, Mur, Boucles, Coupures};
CurrentSection current; 
LCM disp(&DEMO_DDR, &DEMO_PORT);\
void static inline w(void) {
	cp_wait_ms(2000);
}
        
void initialisation(void) {
  cli ();
  EIMSK |= (1 << INT0) | (1 << INT1);

  EICRA |= (1 << ISC00) | (1 << ISC11); 
  sei ();
}

ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        _delay_ms(50);
        if(PIND & 0x04){
            robot.btn.setClicked(1);
        }
    }
    else{
        robot.btn.setClicked(0);
    }
}

ISR(INT1_vect){
    _delay_ms(30);
    if(!(PIND & (1 << PD3))){
        robot.whiteBtnClick = true;
    }
    else {
        robot.whiteBtnClick = false;
    }
    EIFR |= (1 << INTF1);
}

const char* section_char_convert(CurrentSection& current){
    switch(current){
        case Couloir: return "Couloir";
        case Mur: return "Mur";
        case Boucles: return "Boucles";
        case Coupures: return "Coupures";
    }
}

///////////////////////////////////////////////////////
///////////                                 ///////////
///////////               MAIN              ///////////
///////////                                 ///////////
///////////////////////////////////////////////////////
int main(){

    initialisation();
    // bool beginTrack = true;
    // bool init = false;
    // uint8_t sectionSelect = 0;

    // while(!init){
    //     if(robot.btn.getClicked()){
    //         init = true;
    //     }
        
    //     if(robot.whiteBtnClick){
    //         if(sectionSelect >= 4){
    //             sectionSelect = 0;
    //         }
    //         sectionSelect++;
    //     }
    // }

    // switch(sectionSelect){
    //     case 1: current = Couloir; disp = section_char_convert(current); break;
    //     case 2: current = Mur; disp = section_char_convert(current); break;
    //     case 3: current = Boucles; disp = section_char_convert(current); break;
    //     case 4: current = Coupures; disp = section_char_convert(current); break;
    // }

    // initSection = current;

    // switch(current){
    //     case Couloir:
    //         // robot.detect();
    //         // robot.couloir();
    //         // robot.detect();
    //         // robot.avancerCouloirAMur();
    //         robot.del.vert();
    //         _delay_ms(1000);
    //         robot.del.rouge();
    //         while(true){};

    //     case Mur:
    //         // robot.detect();
    //         // robot.mur();
    //         // robot.detect();
    //         // robot.avancerMurABoucles();
    //         robot.del.ambre();
    //         while(true){};

    //     case Boucles:
    //         // robot.detect();
    //         // robot.boucles();
    //         // robot.detect();
    //         // robot.avancerBouclesACoupure();
    //         robot.del.rouge();
    //         while(true){};
        
    //     case Coupures:
    //         // robot.detect();
    //         // robot.coupure();
    //         // robot.detect();
    //         // robot.avancerJusquaCouloir();
    //         // current = Couloir;
    //         robot.del.vert();
    //         while(true){};
    // }

    // robot.stop();
    // robot.moteur.stopEngine();
    // robot.initEndSequence();

    // robot.detect();
    // robot.coupure();
    // robot.detect();
    // robot.avancerCoupureACouloir();
    // robot.detect();
    // robot.avancerJusquaCouloir();
    // robot.detect();
    // robot.couloir();
    // robot.detect();
    // robot.avancerCouloirAMur();
    // robot.detect();
    // robot.mur();
    // robot.detect();
    // robot.avancerMurABoucle();
    robot.detect();
    robot.boucles();
    // robot.avancerBoucleACoupure();
    robot.stop();
    
}
