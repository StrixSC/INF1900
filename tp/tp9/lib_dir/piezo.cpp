#include "piezo.h"

Piezo::Piezo(){
    init();
}

Piezo::~Piezo(){
    //
}

void Piezo::init(){
    TCNT2 = 0; //Set timer2 to 0   
    //On set le timer a fastPWM en mode CTC.
    TCCR2A |= _BV(COM2A0) | _BV(WGM21);                  //CTC mode. Toggle on Compare. 
    TCCR2B |= _BV(CS21);                                 //Prescaler de 8 (Pour quon genere un clock frq de 1MHz)
}

void Piezo::play(uint8_t note){
    OCR2A = calculTemps(listesDesFrequences[note-45]); //On fait -45, car le tableau de frequence commence a la note 45.
    //Donc freq[0] correspond a la note 45. 
}

uint8_t Piezo::calculTemps(uint16_t freq){
    /*
    Methodologie: On sait que la periode et la freq sont relier par la l'eq suivante: T = 1/F
    Or, on sait aussi que le wavelength est toujours de 50%. (Le duty cycle est de 1/2 periode).
    Donc on peut set le duty cycle avec la formule si-dessous. On sais aussi que le prescaler est 256, donc on doit
    diviser le F_CPU avec le prescaler value
    */
   uint8_t dutyCycle = (F_CPU/8)*((1/freq)/2);
   return dutyCycle;
}

    
void Piezo::stop(){
    OCR2A = 0; //On set la duree 0 ;
    TCNT2 = 0; //On set le timer a 0;
    TCCR2B = 0; //On arrete le timer;
}