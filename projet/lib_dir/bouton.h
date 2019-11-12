#ifndef BOUTON_H
#define BOUTON_H


#include "includes.h"

class Bouton {
    public:
        Bouton();
        ~Bouton();

        ////////API///////
        uint8_t click();
        //////////////////

        uint8_t debounce();
        uint8_t getClicked();
        void setClicked(uint8_t val); 

    private:
    uint8_t clicked = 0;
};

#endif