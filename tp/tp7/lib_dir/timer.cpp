#include "timer.h"
/*
<<<<<<< HEAD
@Brief: constructeur, permet dinitialiser les val 
@Param: rien
=======
@Brief: Constructeur par defaut de la classe Timer
@Param: On initialise le mode_ a Normal, le timer_ a TCNT0 (le Timer 0 a 8bit)
la duree que devra prendre 0CR0A a 0 et le prescaler a 1.
On set le OutputMode a Highset, c-a-d que OC1A sera set to high on reach of top par OCR1A.
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
@Return: void
*/
Timer::Timer(){
    mode_ = Normal;
    timer_ = TCNT0_;
    duree_ = 0;
    prescaler_ = P1;
    outputMode_ = HighSet;
}

/*
<<<<<<< HEAD
@Brief: constructeur par parametre 
@Param: Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode
@Return: void
*/

=======
@Brief: Constructeur par param de la classe Timer
Set la valeur des attributs a la valeur donnee par l'utilisateur.
@Param: Un mode (enum), un Timer(enum), une duree (uint8_t), un prescaler (enum)
et un OutputMOde (enum)
@Return: void
*/
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
Timer::Timer(Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode){
    mode_ = mode;
    timer_ = timer;
    duree_ = duree;
    prescaler_ = prescaler;
    outputMode_ = outputMode;
}

<<<<<<< HEAD
/*
@Brief: destructeur 
@Param: rien
@Return: rien
*/

Timer::~Timer(){}

/*
@Brief: initialise le timer
@Param: aucun
@Return: void
*/


=======
//Desturcteur
Timer::~Timer(){}

/*
@Brief: Cette methode permet d'initier le timer avec les attributs
@Param: void
@Return: void
*/

>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
void Timer::start(){
    //On fait appel a chaque methode de modifications pour 
    //s'assurer de set les attributs a ceux que nous avons specifier.
    setInitMode(mode_);
    setPrescaler(prescaler_);
    setDuree(duree_);
    setTimer(timer_);
    setOutputMode(outputMode_);
 
    //Change les Registres par rapport au timer qui est utiliser. 
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
    
    //Enable la routine d'interruption
    sei();
}

/*
<<<<<<< HEAD
@Brief: permet d'arreter le timer 
@Param: rien
@Return: void
*/

=======
@Brief: Cette methode permet d'arreter le timer en marche en ce moment.
@Param: void
@Return: void
*/
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
void Timer::stop(){

    if (timer_ == TCNT0_){
        TCNT0 = 0;  //On set le timer a 0
        TCCR0B = 0; //On set le prescaler a 0 (C-a-d que le timer ne roule plus)
        TIMSK0 &= ~(1 << OCIE0A);   //On retire le mask.
    }
    else if(timer_ == TCNT1_){
        TCNT1 = 0;
        TCCR1B = 0;
        TIMSK1 &= ~(1 << OCIE1A);
    }
    else if(timer_ == TCNT2_){
        TCNT2 = 0;
        TCCR2B = 0;
        TIMSK2 &= ~(1 << OCIE2A);
    }

    //On disable la routine d'interruption.
    cli();
}

/*
<<<<<<< HEAD
@Brief: permet de mettre en parametre la duree de la minuterie
@Param: uint8_t duree
=======
@Brief: Cette methode set que doit avoir OCR1A pour trigger l'interruption
@Param: une duree 8bit
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
@Return: void
*/

void Timer::setDuree(uint8_t duree){

    //On set la duree a celle spceifie par l'utilisateur
    duree_ = duree;

    //Dependamment du timer utiliser, on set les registres approprie
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

<<<<<<< HEAD
/*
@Brief: permet d'indiquer la valeur du timer
@Param: Timers timer
=======

/*
@Brief: Cette methode set le timer au timer qui est donne par l'utilisateur
@Param: un Timer (enum)
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
@Return: void
*/

void Timer::setTimer(Timers timer){
    timer_ = timer;
}

<<<<<<< HEAD
/*
@Brief: Permet d'indiquer le type de timer qu'on veut soit ctc, pmw-pc ou un timer normal. 
@Param: Mode mode
=======


/*
@Brief: Cette methode permet de set le registre TCCR0A au mode que l'on a beosin
Les choix sont les suivants : Normal, CTC, F_PWM, PWM_PC
@Param: un Mode (enum)
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
@Return: void
*/

void Timer::setInitMode(Mode mode){
    mode_ = mode;

    //Dependamment du timer utiliser, on set les registres approprie
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

/*
<<<<<<< HEAD
@Brief: 
@Param: OutputMode m
@Return: void
*/

=======
@Brief: Cette methode permet de set le OutputMode pour le timer.
Les choix que nous avns: NormalOp, HighSet, Lowset ou Toggle.
@Param: un Mode (enum)
@Return: void
*/
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
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

/*
@Brief: genere une minuterie
@Param: Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode
@Return: void
*/

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

<<<<<<< HEAD
/*
@Brief: genere une minuterie
@Param: Mode mode, Timers timer, uint8_t duree,  Prescaler prescaler, OutputMode outputMode
@Return: void
*/

=======
//Cette methode est encore entrain de se faire tester
//Pourrait se faire retirer et ne fait pas partie de l'api entier pour
//l'instant.
>>>>>>> 2de225322e7b7cc3d94638d4436c4a4e346338ef
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

