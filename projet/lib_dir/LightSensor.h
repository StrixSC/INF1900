#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#define SENSOR_LENGTH 5
#define CAPTEUR1 10000
#define CAPTEUR2 1000
#define CAPTEUR3 100
#define CAPTEUR4 10
#define CAPTEUR5 1


#include "includes.h"

class LightSensor {
    public:
    LightSensor();
    ~LightSensor();

    // bool detect(uint16_t toDetect); 
    uint8_t shutdown();

    private:
        //Tableau de 5 espace representant les 5 bits relier aux 5 capteurs.
        bool C1 = false;
        bool C2 = false; 
        bool C3 = false; 
        bool C4 = false;
        bool C5 = false;
    };

#endif