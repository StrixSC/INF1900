#ifndef BOUTON_H
#define BOUTON_H


#include "includes.h"

// enum Mode{scrutation, interrupt};

// class bouton {
// public:   
//     bouton();  
//     ~bouton();

    
//     //////////////////////
//     //API:
//     void click(Mode mode);
//     //////////////////////

//     void debounce();
//     void scruation();
//     void interrupt();
//     void isr();

// private:
//     uint8_t click_ = 0;
// };


class Bouton {
    public:
        Bouton();
        ~Bouton();

        uint8_t debounce();

        uint8_t click();

        uint8_t getClicked();
        void setClicked(uint8_t val); 

    private:
    uint8_t clicked = 0;
};

#endif