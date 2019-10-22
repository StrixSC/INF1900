#include "timer.h"

Timer::Timer(){
    mode_ = Normal;
    timer_ = TCNT0_;
    duree_ = 0;
    prescaler_ = P1;
    outputMode_ = HighSet;
}

Timer::Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode){
    mode_ = mode;
    timer_ = timer;
    duree_ = duree;
    prescaler_ = prescaler;
    outputMode_ = outputMode;
}

Timer::~Timer(){}

void Timer::start(){
    setInitMode(mode_);
    setPrescaler(prescaler_);
    setDuree(duree_);
    setTimer(timer_);
    setOutputMode(outputMode_);
 
    if(timer_ == TCNT0_){
        TCNT0 = 0; 
        TIMSK0 |= (1 << OCIE0A);
    }
    else if (timer_ == TCNT1_){
        TCNT1 = 0;
        TIMSK1 |= (1 << OCIE1A);
    }
    else if (timer_ == TCNT2_){
        TCNT2 = 0;
        TIMSK2 |= (1 << OCIE2A);
    }
    
    sei();
}

void Timer::stop(){

    TCNT0 = 0;
    if (timer_ == TCNT0_){
        TCCR0B = 0;
        TIMSK0 &= ~(1 << OCIE0A);
    }
    else if(timer_ == TCNT1_){
        TCCR1B = 0;
        TIMSK1 &= ~(1 << OCIE1A);
    }
    else if(timer_ == TCNT2_){
        TCCR2B = 0;
        TIMSK2 &= ~(1 << OCIE2A);
    }

    cli();
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

void Timer::setInitMode(Mode mode){
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

void Timer::setOutputMode(OutputMode m){

    /*
    On commence par clear l'OutputMode entierement pour y mettre les nouveaux.
    */

    if(timer_ == TCNT0_){
        TCCR0A &= ~_BV(COM0A1) | ~_BV(COM0A0);
    }
    else if(timer_ == TCNT1_){
        TCCR1A &= ~_BV(COM1A1) | ~_BV(COM1A0);
    }
    else if(timer_ == TCNT2_){
        TCCR2A &= ~_BV(COM2A1) | ~_BV(COM2A0);
    }       

    switch(m){
        case NormalOp:
            if(timer_ == TCNT0_){
                //On ne fait rien
            }
            else if(timer_ == TCNT1_){
                //On ne fait rien
            }
            else if(timer_ == TCNT2_){
                //On ne fait rien
            }
        break;

        case Toggle:
            if(timer_ == TCNT0_){
                TCCR0A |= _BV(COM0A0);
            }
            else if(timer_ == TCNT1_){
                TCCR1A |= _BV(COM1A0);
            }
            else if(timer_ == TCNT2_){
                TCCR2A |= _BV(COM2A0);
            }
        break;

        case LowSet:
            if(timer_ == TCNT0_){
                TCCR0A |= _BV(COM0A1);
            }
            else if(timer_ == TCNT1_){
                TCCR1A |= _BV(COM1A1);
            }
            else if(timer_ == TCNT2_){
                TCCR2A |= _BV(COM2A1);
            }
        break;

        case HighSet:
            if(timer_ == TCNT0_){
                TCCR0A |= _BV(COM0A1) | _BV(COM0A0);
            }
            else if(timer_ == TCNT1_){
                TCCR1A |= _BV(COM1A1) | _BV(COM1A0);
            }
            else if(timer_ == TCNT2_){
                TCCR2A |= _BV(COM2A1) | _BV(COM2A0);
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

void Timer::PWM(uint8_t left, uint8_t right){
    if(mode_ == PWM_PC){
        if(timer_ == TCNT0){
            TCNT0 = 0; 
            OCR0A = left;
            OCR0B = right;
            
        }

        if(timer_ == TCNT1){
            
        }

        if(timer_ == TCNT2){
            
        }
    }
}

