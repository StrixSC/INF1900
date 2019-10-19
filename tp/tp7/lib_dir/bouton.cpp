#include "bouton.h"

// void Bouton::click(Mode mode){
//     switch (mode){
//         case scrutation:
//             scrutation();
//         break;

//         case interrupt:
//             interrupt();
//         break;
//     }
// }

// void Bouton::scrutation() {
//     if(debounce()){
//         click = 1;
//     }
//     else {
//         click = 0;
//     }
// }

// void Bouton::interrupt() {
//     bouton.isr();
// }

// void Bouton::isr() {
//     ISR (INT0_vect) {
//         _delay_ms(30);
//         if(bouton.debounce()){
//             click = 1;
//         }
//         else{
//             click = 0; 
//         }
//     EIFR |= (1 << INTF0);
//     }
// }

    Bouton::Bouton(){}
    Bouton::~Bouton(){}

    uint8_t Bouton::debounce(){
        if(PIND & 0x04){
            _delay_ms(10);
            if(PIND & 0x04){
                setClicked(1);
                return 1;
            }
        }
        else{
            setClicked(0);
            return 0;
        }
    }

    uint8_t Bouton::getClicked() {
        return clicked;
    }

    void Bouton::setClicked(uint8_t val) {
        clicked = val;
    }

    uint8_t Bouton::click(){
        if(debounce()){
            return 1;
        }
        else   
            return 0;
    }