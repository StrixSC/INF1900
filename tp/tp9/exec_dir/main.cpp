#include "../lib_dir/UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/del.h"
#include "../lib_dir/enums.h"
#include "../lib_dir/moteur.h"
#include "../lib_dir/debug.h"

//variables necessaires:
Memoire24CXXX mem;
UART uart;
Port port = B;
Del del;
Moteur moteur;
uint8_t loopAdrStart = 0x00;
uint8_t loopAdrStop = 0x00;
uint8_t loopCounter = 0x00;
uint8_t debuterRoutine = 0;


void prendreAction(uint8_t instruction, uint8_t operande, uint8_t adr);

int main(){

    DDRA = 0x00;
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0x00;
    uint8_t adr = 0x00;
    uint16_t tailleTotal = 0x0000;
    // uint8_t deuxPremiersOctets[] = {0x00, 0x00};
    uint8_t octet1;
    uint8_t octet2; 

    //Obtention de la taille totale:
    mem.lecture(adr, &octet1);
    tailleTotal =  octet1;
    tailleTotal = (tailleTotal << 8);
    adr++;

    mem.lecture(adr, &octet2);
    tailleTotal |= octet2;
    adr++;
        DEBUG_PRINT("Bonjour");
    

    //Nous avons maintenant la taille total du fichier

    while(adr < tailleTotal){
        mem.lecture(adr, &octet1);
        adr++;
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        DEBUG_PRINT("Bonjour");
        mem.lecture(adr, &octet2);
        adr++;
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite

        if(octet1 == 0x01){
            debuterRoutine = 1;
            del.allumerDEL(port);

        }
        if(debuterRoutine){
            prendreAction(octet1, octet2, adr);
        }
    }

}

void prendreAction(uint8_t instruction, uint8_t operande, uint8_t adr){

    switch(instruction){
        case 0x01: //case begin
        break;

        case 0x02: //case Delay
            for(uint8_t i =0; i< operande; i++){    //Attendre 25 fois la valeur de l'operande
                _delay_ms(25);
            }   
        break;

        case 0x44:
            del.allumerDEL(port);
        break;

        case 0x45:
            del.eteindre(port);
        break;

        case 0x48:
            //sonorite.note(operande)
        break;

        case 0x09:
            //sonorite.stop
        break;

        case 0x60:
        case 0x61:  //Stop and start engine
            moteur.startEngine();
            moteur.changeSpeed(0,0);
        break;

        case 0x62:  //Avancer
            moteur.startEngine();
            moteur.changeSpeed(operande,operande);
        break;

        case 0x63:  //Reculer
            moteur.startEngine();
            moteur.changeSpeed(-operande, -operande);
        break;

        case 0x64:  //Tourner Gauche
            moteur.startEngine();
            moteur.changeSpeed(0,50);
        break;

        case 0x65:  //Tourner Droite
            moteur.startEngine();
            moteur.changeSpeed(50,0);
        break;

        case 0xC0:  //Debut For loop
            loopAdrStart = adr;
            loopCounter = operande + 1;
        break;

        case 0xC1:  //Terminer For Loop
            if(loopCounter > 0){
                loopAdrStart--;
                loopCounter--;
            }
        break;

        case 0xFF:  //Terminer Programme
            debuterRoutine = 0;
            // moteur.changeSpeed(0,0);
            // loopAdrStart = 0;
            // loopAdrStop = 0;
            // loopCounter = 0;
        break;
    }
}