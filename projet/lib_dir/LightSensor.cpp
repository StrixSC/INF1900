#include "LightSensor.h"

LightSensor::LightSensor(){ 
    // 
}

LightSensor::~LightSensor(){ 
    // 
}

// void LightSensor::detect(uint16_t toDetect){
//         if(PINA & 0b01){
//             C1 = true;
//         }
//         else if (PINA & 0b10){
//             C2 = true;
//         }
//         else if (PINA & 0b11){
//             C3 = true;
//         }
//         else if (PINA & 0b100){
//             C4 = true;
//         }
//         else if(PINA & 0b101){
//             C5 = true;
//         }
// }

uint8_t LightSensor::shutdown(){
    return 1;
}