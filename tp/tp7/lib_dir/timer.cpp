#include "timer.h"

Timer::Timer(){
    mode_ = Normal;
    timer_ = TCNT0_;
    duree_ = 0;
    prescaler_ = P1;
}

Timer::Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler){
    mode_ = mode;
    timer_ = timer;
    duree_ = duree;
    prescaler_ = prescaler;
}

Timer::~Timer(){}

void Timer::start(){
    setMode(mode_);
    setPrescaler(prescaler_);
    setDuree(duree_);
    setTimer(timer_);

    TCNT0 = 0;
    TIMSK1 = (1 << OCIE1B);
}

void Timer::stop(){

    TCNT0 = 0;
    if (timer_ == TCNT0_){
        TCCR0B == 0;
    }
    else if(timer_ == TCNT1_){
        TCCR1B == 0;
    }
    else if(timer_ == TCNT2_){
        TCCR2B == 0;
    }
}

void Timer::setDuree(uint8_t duree){
    duree_ = duree;
    if(timer_ == TCNT0_){
        OCR0A = duree;
    }
    else if (timer_ == TCNT1_){
        OCR1A = duree;
    }
    else if (timer_ == TCNT2_){
        OCR2A = duree;
    }
}

void Timer::setTimer(Timers timer){
    timer_ = timer;
}

void Timer::setMode(Mode mode){
    mode_ = mode;

    switch (mode){
        case Normal:
            if (timer_ == TCNT0_){
                TCCR0A = 0;
            }
            else if (timer_ == TCNT1_){
                TCCR1A = 0;
            }
            else if (timer_ == TCNT2_){
                TCCR2A = 0;
            }
        break;

        case CTC:
            if (timer_ == TCNT0_){
                TCNT0 = 
                TCCR0A = (1 << WGM01);
            }
            else if (timer_ == TCNT1_){
                TCCR1A = (1 << WGM12);
            }
            else if (timer_ == TCNT2_){
                TCCR2A = (1 << WGM21);
            }
        break;

        case PWM_PC:
            if (timer_ == TCNT0_){
                TCCR0A = (1 << WGM00) | (1 << WGM02);
            }
            else if (timer_ == TCNT1_){
                TCCR1A = (1 << WGM10) | (1 << WGM11) | (1 << WGM13);
            }
            else if (timer_ == TCNT2_){
                TCCR2A = (1 << WGM20) | (1 << WGM22);
            }
        break;

        case FPWM_8:
            if (timer_ == TCNT0_){
                TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << WGM02);
            }
            else if (timer_ == TCNT1_){
                TCCR1A = (1 << WGM10) | (1 << WGM12);
            }
            else if (timer_ == TCNT2_){
                TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << WGM22);
            }
        break;
    }
}

void Timer::setPrescaler(Prescaler pres){
    prescaler_ = pres;
    switch(pres){
        case P1:
            if(timer_ == TCNT0_){
                TCCR0B = (1 << CS00);
            }
            else if(timer_ == TCNT1_){
                TCCR1B = (1 << CS10);
            }
            else if(timer_ == TCNT2_){
                TCCR2B = (1 << CS20);
            }
        break;

        case P8:
            if(timer_ == TCNT0_){
                TCCR0B = (1 << CS01);
            }
            else if(timer_ == TCNT1_){
                TCCR1B = (1 << CS11);
            }
            else if(timer_ == TCNT2_){
                TCCR2B = (1 << CS21);
            }
        break;

        case P64:
            if(timer_ == TCNT0_){
                TCCR0B = (1 << CS01) | (1 << CS00);
            }
            else if(timer_ == TCNT1_){
                TCCR1B = (1 << CS11) | (1 << CS10);
            }
            else if(timer_ == TCNT2_){
                TCCR2B = (1 << CS21) | (1 << CS20);
            }
        break;
    
        case P256:
            if(timer_ == TCNT0_){
                TCCR0B = (1 << CS02);
            }
            else if(timer_ == TCNT1_){
                TCCR1B = (1 << CS12);
            }
            else if(timer_ == TCNT2_){
                TCCR2B = (1 << CS22);
            }
        break;

        case P1024:
            if(timer_ == TCNT0_){
                TCCR0B = (1 << CS00) | (1 << CS02);
            }
            else if(timer_ == TCNT1_){
                TCCR1B = (1 << CS10) | (1 << CS12);
            }
            else if(timer_ == TCNT2_){
                TCCR2B = (1 << CS20) | (1 << CS22);
            }
        break;
    }

}



