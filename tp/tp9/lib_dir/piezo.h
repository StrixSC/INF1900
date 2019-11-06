#ifndef PIEZO_H
#define PIEZO_H
#include "includes.h"

class Piezo {

    public:

        Piezo();
        ~Piezo();

        //API//
        void play(uint8_t note);
        void stop();

        //BACKEND//
        void init();

    private:
        //La liste des frequences a utiliser pour le piezo. Fournit par le professeur.
        uint16_t listesDesFrequences[37] = { 
                            110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195, 207, 220, 233, 246, 261, 277, 293, 311, 
                            329, 349, 369, 391, 415, 440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830, 880 
                        };
        
};
#endif