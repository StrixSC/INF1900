#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib_dir/bouton.h"
#include "../lib_dir/timer.h"
// #include "../lib_dir/del.h"
// #include "../lib_dir/moteur.h"

Bouton bouton = Bouton();
volatile uint8_t minuterieExpiree = 0; 
uint8_t d = 31250;
// Comment initier les timers:
// Timer (MODE , TIMER QUE L'ON VEUT UTILISER, DUREE DU TIMER, PRESCALER)
// CHOIX DE TIMERS:     TCNT0_(8Bits), TCNT1_ (16Bits), TCNT2_(8Bits)
// CHOIX DE MODES:      Normal, PWM_PC, CTC, FPWM_8
// CHOIX DE PRESCALERS: P1, P8, P64, P256, P1024

Timer timer(Normal, TCNT1_, d, P1024);

void initialisation ( void ) {
    cli();
    DDRB = 0xFF;
    DDRD = 0x00;
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    sei ();
}

ISR(TIMER1_COMPA_vect){
    minuterieExpiree = 1;
    timer.stop();
}

int main(){

    TCNT1 |= (1 << COM1A1) | (1 << COM1A0);
    initialisation();
    timer.start();
    for(;;){
        if(minuterieExpiree == 1)
            PORTB = 0b00;
        else 
            PORTB = 0b01;
    }

    return 0;

}