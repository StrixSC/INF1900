#include "bouton.h"

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