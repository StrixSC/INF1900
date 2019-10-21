#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib_dir/bouton.h"
#include "../lib_dir/timer.h"
#include "../lib_dir/del.h"
#include "../lib_dir/can.h"
#include "../lib_dir/memoire_24.h"
// #include "../lib_dir/moteur.h"
 
// Comment initier les timers:
// Timer (MODE , TIMER QUE L'ON VEUT UTILISER, DUREE DU TIMER, PRESCALER)
// CHOIX DE TIMERS:     TCNT0_(8Bits), TCNT1_ (16Bits), TCNT2_(8Bits)
// CHOIX DE MODES:      Normal, PWM_PC, CTC, FPWM_8
// CHOIX DE PRESCALERS: P1, P8, P64, P256, P1024
// CHOIX DE OUTPUTMODE: NormalOp, Toggle, LowSet, HighSet

// Exemple: Timer timer(Normal, TCNT1_, d, P1024, HighSet);

// enum Timers { TCNT0_, TCNT1_, TCNT2_ };
// enum Mode {Normal, PWM_PC, CTC, FPWM_8};
// enum Prescaler {P1, P8, P64, P256, P1024};
// enum Couleur {Rouge, Vert, Ambre};
// enum Port {A, B, C, D};
// enum OutputMode {NormalOp, Toggle, LowSet, HighSet};

Port b = B;                 //Choix du port pour de la Del
Del del = Del();            //Init d'un nouvel objet Del
Bouton bouton = Bouton();   //Init d'un nouvel objet Bouton

Timers t = TCNT1_;
Mode m = Normal;
Prescaler p = P1024;
OutputMode op = HighSet;
uint8_t duree = 10000;

Timer timer1 = Timer(m, t, duree,p, op);
volatile uint8_t minuterieExpiree = 0;

ISR(TIMER1_COMPA_vect){
    del.allumerDEL(b);
    timer1.stop();
}

int main(){
    timer1.start();
    DDRD = 0x00;
    DDRB = 0xFF;
    for(;;){
       
    }

    return 0;

}