#include "Robot.h"

Robot::Robot(){
    DDRA |= _BV(PA6);
    DDRB = 0xFF; 
    DDRD = 0xF0;
    moteur.startEngine();
}

Robot::~Robot(){
    //
}


void Robot::detect(){
    if(PINA & CAPTEUR1)
        C1 = true;
    else if(!(PINA & CAPTEUR1))
        C1 = false;
        
    if(PINA & CAPTEUR2)
        C2 = true;
    else if(!(PINA & CAPTEUR2))
        C2 = false;
        
    if(PINA & CAPTEUR3)
        C3 = true;
    else if(!(PINA & CAPTEUR3))
        C3 = false;
    
    if(PINA & CAPTEUR4)
        C4 = true;
    else if(!(PINA & CAPTEUR4))
        C4 = false;
    
    if(PINA & CAPTEUR5)
        C5 = true;
    else if(!(PINA & CAPTEUR5))
        C5 = false;
}

void Robot::initEndSequence(){
    del.rouge();
    _delay_ms(ONE_SECOND);
    del.eteindre();
    _delay_ms(ONE_SECOND);
    del.rouge();
    _delay_ms(ONE_SECOND);
    del.eteindre();
    _delay_ms(ONE_SECOND);
}

void Robot::followLine(){
    bool detected = true;
    while(detected){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                detected = false;
            }
        }
        else if(C3 || (C2 && C3 && C4)){         
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(C2 || (C1 && C2)){              
            moteur.changeSpeed(SUBSPEED, AVGSPEED);
        }
        else if(C4 || (C4 && C5)){              
            moteur.changeSpeed(AVGSPEED, SUBSPEED);
        }
        else if(C1){                            
            moteur.changeSpeed(NOSPEED, AVGSPEED);
        }
        else if(C5){                            
            moteur.changeSpeed(AVGSPEED, NOSPEED);  
        }
    }
}

void Robot::bouclesFL(){
    detect();
    if(C3 || (C2 && C3 && C4)){         
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C2 || (C1 && C2)){              
        moteur.changeSpeed(SUBSPEED, AVGSPEED);
    }
    else if(C4 || (C4 && C5)){              
        moteur.changeSpeed(AVGSPEED, SUBSPEED);
    }
    else if(C1){                            
        moteur.changeSpeed(NOSPEED, AVGSPEED);
    }
    else if(C5){                            
        moteur.changeSpeed(AVGSPEED, NOSPEED);  
    }
}

void Robot::stop(){
    moteur.changeSpeed(NOSPEED, NOSPEED);
}


void Robot::turnSequence(const char direction){
    if(direction == 'r'){
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C3 || C4){
                turnBool = false;
            }
            else{
                del.rouge();
                moteur.turnRight(HIGHSPEED);
                detect();
            }
        }
        // rightTurnCounter--;
    }
    else if(direction == 'l'){
        uint8_t turnBool = 1;
        while(turnBool){
            detect();
            if(C2 || C3){
                turnBool = false;
            }
            else{
                del.vert();
                moteur.turnLeft(HIGHSPEED);
                detect();
            }
        }
    }
    else {              //Si le input de l'utilisateur n'est pas L ou R. 
        return;
    }
}

void Robot::basicTurnSequence(const char direction){
    if(direction == 'r'){
        bool turnRight = true;
        while(turnRight){
            if(C3 || C4 || C5){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(C3 || C4 || C5){
                    turnRight = false;
                }
            }
            else {
                moteur.changeSpeed(AVGSPEED, NOSPEED);
            }
        }
    }
    else if(direction == 'l'){
        bool turnLeft = true;
        while(turnLeft){
            if(C1 || C2 || C3){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(C1 || C2 || C3){
                    turnLeft = false;
                }
            }
            else {
                moteur.changeSpeed(NOSPEED, AVGSPEED);
            }
        }
    }
    else{   //Direction == NULL
        return;
    }
}
void Robot::sonarSendPulse(){
    PORTA |= _BV(PA6);   
    _delay_us(15);
    PORTA &= ~_BV(PA6);
}

void Robot::sonarReadOutput(){
    while(!(PINA & _BV(PA7))){}
    uint16_t counter = 0;
    while((PINA & _BV(PA7))){
        _delay_us(0.5);
        counter++;
    };
    distance = (counter/58);
}

char Robot::boucles_ctr_char_converter(const uint8_t num){
    switch(num){
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return '10';
        case 11: return '11';
        case 12: return '12';
        case 13: return '13';
    }
}  

/////////////////////////////////////////////////////////////////////////////////////
//Parcours:
void Robot::avancerJusquaCouloir(){
    bool detected = true;
    while(detected){
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                del.vert();
                turnSequence('l');
                detected = false;
            }
        }
        else{
            followLine();
        }
    }
    bool preCouloir = true;

    while(preCouloir){
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                preCouloir = false;
            }
        }
        else {
            followLine();
        }
    }
}

void Robot::couloir(){
    bool couloir = true;
    bool couloirLeanRight;
    bool couloirLeanLeft;
    uint32_t counter = 0;
    uint32_t previousCounter = 0;
    
    bool tempFL = true;
    while(tempFL){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            tempFL = false;
        }
        else{
            followLine();
        }
    }
    moteur.changeSpeed(HIGHSPEED, NOSPEED);
    while(couloir){
        detect();
        
        if(previousCounter != 0 && counter < previousCounter/2){
            couloir = false;
            break;
        }
        else{
            previousCounter = counter;
            counter = 0;
        }

        if(C1 || (C1 && C2)){
            couloirLeanLeft = false;
            couloirLeanRight = true;
        }
        else if(C5 || (C4 && C5)){
            couloirLeanLeft = true;
            couloirLeanRight = false;
        }

        while(couloirLeanLeft){
            detect();
            if(C1 || (C1 && C2)){
                couloirLeanLeft = false;
            }
            moteur.changeSpeed(NOSPEED, HIGHSPEED);
            counter++;
        }
        while(couloirLeanRight){
            detect();
            if(C5 || (C4 && C5)){
                couloirLeanRight = false;
            }
            moteur.changeSpeed(HIGHSPEED, NOSPEED);
            counter++;
        }
    }
}

void Robot::avancerCouloirAMur(){
    detect();
    bool detected = true;
    while(detected){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                turnSequence('l');
                detected = false;
            }
        }
        else{
            followLine();
        }
    }

    bool preMur = true;
        while(preMur){
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                _delay_ms(DEBOUNCE_DELAY);
                detect();
                if(!C1 && !C2 && !C3 && !C4 && !C5){
                    preMur = false;
                }
            }
            else {
                detect();
                followLine();
            }
        }
    }

void Robot::mur(){
    bool mur = true;
    while(mur){
        detect();
        if(C1 || C2 || C3 || C4 || C5){
            mur = false;
        }

        sonarSendPulse();
        sonarReadOutput();
        _delay_ms(50);

        if(distance >=13  && distance <= 17){
            moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(distance < 13){
            moteur.changeSpeed(SUBSPEED,0);
        }
        else if(distance > 17){
            moteur.changeSpeed(0, SUBSPEED);
        }
    }
}


void Robot::avancerMurABoucles(){}

void Robot::boucles(){
    uint8_t bclCounter = 0; 
    bool bcl = true;
    while(bcl){
        uart.transmissionUART(bclCounter);
        detect();
        if(C1 && C2 && C3 || (C1 && C2 && C3 && C4)){
            _delay_ms(100);
            detect();
            if(C1 && C2 && C3  || (C1 && C2 && C3 && C4) ){
                moteur.changeSpeed(AVGSPEED, AVGSPEED);
                _delay_ms(DEBOUNCE_DELAY);
                bclCounter++;
            }
        }
        else if(C3 || (C2 && C3 && C5)){
            moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(C2){
            moteur.changeSpeed(AVGSPEED, HIGHSPEED);
        }
        else if(C4){
            moteur.changeSpeed(HIGHSPEED, AVGSPEED);
        }
        else if(C5 || C4 && C5){
            moteur.changeSpeed(AVGSPEED, NOSPEED);
        }
        else if(C1 || C2 && C1){
            moteur.changeSpeed(NOSPEED, AVGSPEED);
        }

        switch(bclCounter){
            case 0: break;
            case 1: break;
            case 2: break;
            case 3: turnSequence('l'); break;
            case 4: turnSequence('l'); break;
            case 5: turnSequence('l'); break;
            case 6: turnSequence('l'); break;
            case 7: turnSequence('l'); break;
            case 8: turnSequence('l'); break;
            case 9: turnSequence('l'); break;
            case 10: turnSequence('l'); break;
            case 11: break;
            case 12: bcl = false; break;
        }
    }
    del.vert();
}

void Robot::avancerBouclesACoupure(){}

void Robot::avancerJusquaProchaineCoupure(){
    bool coupureBool = true;
    while(coupureBool){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                coupureBool = false;
            }
        }
        else{
            if(C3 || (C2 && C3 && C4)){
                moteur.changeSpeed(AVGSPEED, AVGSPEED);
            }
            else if(C1 || C2 || (C1 && C2)){
                moteur.changeSpeed(NOSPEED, AVGSPEED);
            }
            else if(C4 || C5 || (C4 && C5)){
                moteur.changeSpeed(AVGSPEED, NOSPEED);
            }
        }
    }
}
void Robot::coupure(){
    avancerJusquaProchaineCoupure();
    basicTurnSequence('r');
    avancerJusquaProchaineCoupure();
    basicTurnSequence('l');
    avancerJusquaProchaineCoupure();
    basicTurnSequence('r');
    avancerJusquaProchaineCoupure();
    basicTurnSequence('l');
}
void Robot::avancerCoupureACouloir(){}