#ifndef BOUTON_H
#define BOUTON_H

#include "includes.h"

enum Mode{scrutation, interrupt};

Class bouton {
public:   
    bouton();  
    ~bouton();

    
    //////////////////////
    //API:
    void click(Mode mode);
    //////////////////////

    void debounce();
    void scruation();
    void interrupt();
    void isr();

private:
    uint8_t click = 0;
};


#endif BOUTON_H