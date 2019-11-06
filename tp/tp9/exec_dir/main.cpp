/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
Description: Ce fichier permet de faire passer au robot différents états pour lui faire faire ce que nous désirons en fonction de ce qu'il lui est passé en paramètres
            grâce aux états du switch case.
Fichiers utilisés: Uart.h, includes.h, memoire_24.h, del.h, enums.h, moteur.h

**************************************************************************************************************************/

#include "UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/del.h"
#include "../lib_dir/enums.h"
#include "../lib_dir/moteur.h"
#include "../lib_dir/piezo.h"


#define DBT 0x01
#define ATT 0x02
#define DAL 0x44
#define DET 0x45
#define SGO 0x48
#define SAR 0x09
#define MAR 0x60
#define MAR2 0x61
#define MAV 0x62
#define MRE 0x63
#define TRD 0x64
#define TRG 0x65
#define DBC 0xC0
#define FBC 0xC1
#define FIN 0xFF

//variables necessaires:
Memoire24CXXX mem;
UART uart;
Del del;
Moteur moteur;
Piezo piezo;

uint8_t loopAdrStart = 0x00;
uint8_t loopAdrStop = 0x00;
uint8_t loopCounter = 0x00;
uint8_t debuterRoutine = 0;
uint8_t read = 0;
uint16_t adr = 0x00;
void prendreAction(uint8_t instruction, uint8_t operande);
void seanceInit();

int main(){
    //nous définissons nos entrées et nos sorties
    DDRA = 0x00; //mode entrée
    DDRB = 0xFF; //mode sortie
    DDRD = 0xFF;
    DDRC = 0x00;

    uint16_t tailleTotal = 0x0000;
    uint8_t octet1; ///8 bit, car nous avons besoin d'un octet
    uint8_t octet2; 

    //Obtention de la taille totale:
    mem.lecture(adr, &octet1);
    tailleTotal =  octet1;
    tailleTotal = (tailleTotal << 8); // décalage de 8 bits vers la gauche pour après faire un or avec le second octet qui nous permettra d'avoir une valeur sur 16bits
    adr++; // on passe à l'adresse suivante

    //on fait appelle au à la fonction lecture de la memoire_24.h qui permet de lire une adresse et le stocker dans une variable
    mem.lecture(adr, &octet2);
    tailleTotal |= octet2;//permet de faire le ou logique pour obtenir la taille du fichier qui est donné par les 2 premiers octets
    adr++;

    //Nous avons maintenant la taille total du fichier

    //Debut de la sceance d'initiation:
    seanceInit();

    while(adr < tailleTotal){
        mem.lecture(adr, &octet1);
        adr++;
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        mem.lecture(adr, &octet2);
        adr++;
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite

        if(octet1 == 0x01){ // permet de débuter la routine, seulement si on se trouve au début de l'adresse
            debuterRoutine = 1;
        }
        if(debuterRoutine){
            prendreAction(octet1, octet2);
        }
    }
    return 0;
}

void prendreAction(uint8_t instruction, uint8_t operande){

    switch(instruction){
        case DBT: //case begin
        break;

        case ATT: //case Delay
            for(uint8_t i =0; i< operande; i++){    //Attendre 25 fois la valeur de l'operande
                _delay_ms(25);
            }   
        break;

        case DAL:
            del.vert();
        break;

        case DET:
            del.eteindre();
        break;

        case SGO:
            piezo.play(operande);
        break;

        case SAR:
            piezo.stop();
        break;

        case MAR:
        case MAR2:  //Stop and start engine
            moteur.changeSpeed(0,0);
        break;

        case MAV:  //Avancer
            moteur.changeSpeed(operande,operande);
        break;

        case MRE:  //Reculer
            moteur.changeSpeed(-operande, -operande);
        break;

        case TRG:  //Tourner Gauche
            moteur.changeSpeed(0,50);
        break;

        case TRD:  //Tourner Droite
            moteur.changeSpeed(50,0);
        break;

        case DBC:
            loopAdrStart = adr;
            loopCounter = operande + 1;
        break;

        case FBC:  
            if(loopCounter > 0){
                adr = loopAdrStart;
                loopCounter--;
            }
        break;

        case FIN:  //Terminer Programme
            debuterRoutine = 0;
            // moteur.changeSpeed(0,0);
            // loopAdrStart = 0;
            // loopAdrStop = 0;
            // loopCounter = 0;
        break;
    }
}

void seanceInit(){
   for(uint8_t i =0; i<2; i++){
       del.rouge();
       _delay_ms(500);
       del.eteindre();
       _delay_ms(500);
   }

   for(uint8_t i =0; i<2; i++){
       del.vert();
       _delay_ms(500);
       del.eteindre();
       _delay_ms(500);
   }

}