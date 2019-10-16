#include "bouton.h"

Bouton::bouton(){
}

void Bouton::click(Mode mode){
    switch (mode){
        case scrutation:
            scrutation();
        break;

        case interrupt:
            interrupt();
        break;
    }
}

void Bouton::scrutation() {
    if(debounce()){
        click = 1;
    }
    else {
        click = 0;
    }
}

void Bouton::interrupt() {
    bouton.isr();
}

void Bouton::isr() {
    ISR (INT0_vect) {
        _delay_ms(30);
        if(bouton.debounce()){
            click = 1;
        }
        else{
            click = 0; 
        }
    EIFR |= (1 << INTF0);
    }
}