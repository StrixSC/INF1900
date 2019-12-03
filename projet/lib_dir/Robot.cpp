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
void Robot::basicFollowLine(){
    detect();
    if((C2 && C3) || (C2 && C1)){
        moteur.changeSpeed(SUBSPEED, AVGSPEED);
    }
    else if ((C4 && C3) || (C4 && C5)){
        moteur.changeSpeed(AVGSPEED, SUBSPEED);
    }
    else if(C3 || (C2 && C3 && C4)){
        moteur.changeSpeed(AVGSPEED, AVGSPEED);
    }
    else if(C1){
        moteur.changeSpeed(NOSPEED, SUBSPEED);
    }
    else if(C5){
        moteur.changeSpeed(AVGSPEED, NOSPEED);
    }
}

void Robot::boost(){
    moteur.changeSpeed(HIGHSPEED, HIGHSPEED);
    _delay_ms(50);
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
                _delay_ms(100);
                detect();
                if(C3 || C4){
                    turnBool = false;
                }
            }
            else{
                del.rouge();
                moteur.turnRight(SUBSPEED);
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
                _delay_ms(100);
                detect();
                if(C2 || C3){
                    turnBool = false;
                }
            }
            else{
                del.vert();
                moteur.turnLeft(SUBSPEED);
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
                detect();
                if(C3 || C4 || C5){
                    _delay_ms(40);
                    detect();
                    if(C3 || C4 || C5){
                        turnRight = false;
                    }
                }
                else {
                    moteur.changeSpeed(45, NOSPEED);
                }
            }
            boost();
    }
    else if(direction == 'l'){
        bool turnLeft = true;
        while(turnLeft){
            detect();
            if(C1 || C2 || C3){
                _delay_ms(60);
                detect();
                if(C1 || C2 || C3){
                    turnLeft = false;
                }
            }
            else {
                moteur.changeSpeed(NOSPEED, 45);
            }
        }
        boost();
    }
    else{   //Direction == NULL
        return;
    }

}
void Robot::boucleTurnSequence(const char direction){
    if(direction == 'r'){
        bool turnRight = true;
        while(turnRight){
            detect();
            if(!C5 && !C4 && C3){
                _delay_ms(75);
                detect();
                if(C3 || (C4 && C3)){
                    turnRight = false;
                }
            }
            else {
                moteur.changeSpeed(AVGSPEED, NOSPEED);
            }
        }
        boost();
    }
    else if(direction == 'l'){
        bool turnLeft = true;
        while(turnLeft){
            detect();
            if(!C1 && !C2 && C3){
                _delay_ms(75);
                detect();
                if(C3 || (C2 && C3)){
                    turnLeft = false;
                }
            }
            else {
                moteur.changeSpeed(NOSPEED, AVGSPEED);
            }
        }
        boost();
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
    
    moteur.changeSpeed(AVGSPEED, NOSPEED);
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
            moteur.changeSpeed(NOSPEED, AVGSPEED);
            counter++;
        }
        while(couloirLeanRight){
            detect();
            if(C5 || (C4 && C5)){
                couloirLeanRight = false;
            }
            moteur.changeSpeed(AVGSPEED, NOSPEED);
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
                moteur.changeSpeed(45, 45);
                _delay_ms(300);
                turnSequence('l');
                detected = false;
            }
        }
        else{
            followLine();
        }
    }
}

void Robot::preMur(){
    bool premur = true;
    while(premur){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(50);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                premur = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
}

void Robot::mur(){
    bool mur = true;
    while(mur){
        detect();
        if(C1 || C2 || C3 || C4 || C5){
                basicTurnSequence('r');
                _delay_ms(DEBOUNCE_DELAY);
                mur = false;
            }
        sonarSendPulse();
        sonarReadOutput();
        _delay_ms(50);

        if(distance >=14  && distance <= 19){
            moteur.changeSpeed(AVGSPEED, AVGSPEED);
        }
        else if(distance < 14){
            moteur.changeSpeed(AVGSPEED,NOSPEED);
        }
        else if(distance > 19){
            moteur.changeSpeed(NOSPEED, AVGSPEED);
        }
    }
}

void Robot::avancerMurABoucles(){
    bool murAboucle = true;
    while(murAboucle){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                moteur.changeSpeed(AVGSPEED, AVGSPEED);
                _delay_ms(DEBOUNCE_DELAY);
                basicTurnSequence('l');
                murAboucle = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
    boost();
}

void Robot::boucleCheck(){
    bool bcl = true;
    while(bcl){
        detect();
        if((C1 && C2 && C3 )|| (C1 && C2 && C3 && C4) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4 && C5)){
            _delay_ms(110);
            detect();
            if((C1 && C2 && C3 ) || (C1 && C2 && C3 && C4) || (!C1 && !C2 && !C3 && !C4 && !C5) || (C1 && C2 && C3 && C4 && C5)){
                bcl = false;
            }
        }
        else {
            basicFollowLine();
        }
    }
}
void Robot::boucles(){
    moteur.changeSpeed(AVGSPEED, SUBSPEED);
    _delay_ms(50);
    uint16_t counter = 0;
    del.vert();
    while(counter <= 12){
        detect();
        boucleCheck();
        counter++;
        switch(counter){
            case 0: 
                del.vert();
            break;
            case 1: 
                del.rouge();
            break;
            case 2: 
                del.vert();
            break;
            case 3: 
                del.rouge();
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 4: 
                del.vert();
                moteur.changeSpeed(SUBSPEED, SUBSPEED);
                _delay_ms(100);
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 5: 
                del.rouge();
                moteur.changeSpeed(SUBSPEED, SUBSPEED);
                _delay_ms(100);
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 6: 
                del.vert();
                moteur.changeSpeed(SUBSPEED, SUBSPEED);
                _delay_ms(100);
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 7: 
                del.rouge();
                boucleTurnSequence('l'); 
            break;
            case 8: 
                del.vert();
                boucleTurnSequence('l'); 
            break;
            case 9: 
                del.rouge();
                moteur.changeSpeed(SUBSPEED, SUBSPEED);
                _delay_ms(100);
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 10: 
                del.vert();
                moteur.changeSpeed(SUBSPEED, SUBSPEED);
                _delay_ms(100);
                moteur.changeSpeed(NOSPEED, AVGSPEED);
                _delay_ms(100);
                boucleTurnSequence('l'); 
            break;
            case 11: break;
            case 12: return;
        }
    }
}
void Robot::avancerBouclesACoupure(){
    bool boucleACoupure = true;
    while(boucleACoupure){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                moteur.changeSpeed(AVGSPEED, AVGSPEED);
                _delay_ms(HALF_SECOND);
                turnSequence('l');
                boucleACoupure = false;
            }
        }
        else{
            basicFollowLine();
        }
    }
}

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
            basicFollowLine();
        }
    }
}
void Robot::coupure(){
    avancerJusquaProchaineCoupure();
    piezo.play(80); //Son aigue
    basicTurnSequence('r');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(50); //Son grave
    basicTurnSequence('l');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(80); //Son aigue
    basicTurnSequence('r');
    piezo.stop();
    avancerJusquaProchaineCoupure();
    piezo.play(50); //Son grave
    basicTurnSequence('l');
    piezo.stop();
}

void Robot::avancerCoupureACouloir(){
    bool coupureACouloir = true;
    while(coupureACouloir){
        detect();
        if(!C1 && !C2 && !C3 && !C4 && !C5){
            _delay_ms(DEBOUNCE_DELAY);
            detect();
            if(!C1 && !C2 && !C3 && !C4 && !C5){
                moteur.changeSpeed(AVGSPEED, AVGSPEED);
                _delay_ms(DEBOUNCE_DELAY);
                coupureACouloir = false;
            }
        }
        else{
           basicFollowLine();
        }
    }
}