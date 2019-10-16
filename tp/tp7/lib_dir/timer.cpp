#include "timer.h"

Timer::Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler):  {
    mode_ = mode;
    timer_ = timer;
    duree_ = duree;
    prescaler_ = prescaler;
Timer::start(){
    switch (mode){
        case Normal:
            
        break;

        case CTC:
            
        break;
    }

}

Timer::setPrescaler(){

    switch(prescaler){
        case 1:
        
        break;

        case 2:

        break
    }

}



