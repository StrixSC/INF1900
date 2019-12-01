#include "piezo.h"

Piezo::Piezo(){
    // init();
}

Piezo::~Piezo(){
    //
}

void Piezo::init(){
    //On set le timer a fastPWM en mode CTC.
    TCCR0A |= _BV(COM0A0) | _BV(WGM01);                  //CTC mode. Toggle on Compare. 
    TCCR0B |= _BV(CS02);                                 //Prescaler de 256 (Pour quon genere un clock frq de 1MHz)
}

void Piezo::stop(){
    OCR0A = 0; //On set la duree 0 ;
    TCNT0 = 0; //On set le timer a 0;
    TCCR0B = 0; //On arrete le timer;
}

void Piezo::play(uint8_t key){
    float frequence = listesDesFrequences[key-45]; //-45, car la premiere note du tableau est la note 45
    uint8_t dutyCycle = (F_CPU/256)*((1/frequence)/2); 

    /*
    Methodologie: On sait que la periode et la freq sont relier par la l'eq suivante: T = 1/F
    Or, on sait aussi que le wavelength est toujours de 50%. (Le duty cycle est de 1/2 periode).
    Donc on peut set le duty cycle avec la formule si-dessous. On sais aussi que le prescaler est 256, donc on doit
    diviser le F_CPU avec le prescaler value
    */

    init();
    OCR0A = dutyCycle; 
}

void Piezo::beep(){
        play(65);
        _delay_ms(500);
        play(65);
        _delay_ms(500);
}

void Piezo::playSpongeBob(){
    uint8_t song = 1;
    while (song){
        /////////////////////
        //Who-Lives-In-A-Pine-Ap-ple-Under-The-Sea
        play(74);
        _delay_ms(250);

        play(74);
        _delay_ms(250);

        play(76);
        _delay_ms(250);
        
        play(74);
        _delay_ms(250);

        play(71);
        _delay_ms(250);

        play(68);
        _delay_ms(250);

        play(71);
        _delay_ms(250);

        play(74);
        _delay_ms(250);
                
        play(76);
        _delay_ms(250);

        play(74);
        _delay_ms(300);
        /////////////////////
        //Sponge-Bob-Square-Pants
        play(71);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(250);
        ////////////////////
        //Lyrics I dont know
        play(79);
        _delay_ms(250);

         play(79);
        _delay_ms(250);

        play(81);
        _delay_ms(250);
        
        play(79);
        _delay_ms(250);

        play(76);
        _delay_ms(250);

        play(72);
        _delay_ms(250);

        play(76);
        _delay_ms(250);

        play(79);
        _delay_ms(250);
                
        play(81);
        _delay_ms(250);

        play(79);
        _delay_ms(300);
        /////////////////////
        //Sponge-Bob-Square-Pants
        play(76);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(300);
        /////////////////////
        //SPONGE-BOB-SQUARE-PANTS!
        play(79);
        _delay_ms(500);

        play(76);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(500);
        /////////////////////
        play(79);
        _delay_ms(500);

        play(76);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(500);
        /////////////////////
        play(79);
        _delay_ms(500);

        play(76);
        _delay_ms(500);

        play(74);
        _delay_ms(500);

        play(71);
        _delay_ms(500);

        /////////////////////
        //FINALE
        play(74);
        _delay_ms(500);

        play(76);
        _delay_ms(1250);

        play(78);
        _delay_ms(500);

        play(79);
        _delay_ms(1500);

        stop();
        song = 0;
    }
}