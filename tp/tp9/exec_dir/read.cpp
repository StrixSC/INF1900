#include "../lib_dir/UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/del.h"
#include "../lib_dir/enums.h"


void prendreAction(uint8_t instruction);

//nous créons des variables de type de chacune des classes que nous allons utliser
UART uart;
Memoire24CXXX memoire;
uint16_t positionInitiale=0x00;
uint8_t tailleOctet;
uint8_t debuterRoutine = 0;
Port port = B;
Del del;


int main(){
    DDRB = 0xFF;
    //Nous devons lire les 2 premiers bits de la mémoire, car ce sont ces bits que nous avons écrits dans la mémoire externe
    for(int i=0; i<2; i++){
        memoire.lecture(positionInitiale, &tailleOctet);
        positionInitiale++;
    }

    //Taille des octets pour savoir le nombre de fois que la boucle while va s executer pour ecrire
    tailleOctet=tailleOctet& (1<<tailleOctet); //formule prise de la documentation du atmega à la page 27 du document
    
    //boucle pour recevoir et ecrire les donnees dans la memoire
    while(positionInitiale < tailleOctet){
        del.allumerDEL(port);
        //Pour pouvoir transmettre une donnée en mémoire externe, nous devons d'abord en recevoir une. La donnée provient de uart, donc nous devons la recevoir
        uint8_t instruction = memoire.lecture(positionInitiale, &tailleOctet);//lit a la position initiale la donnee qui a été recue a partir de uart
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        if(instruction == 0x01){
            debuterRoutine = 1;
            del.allumerDEL(port);
        }
        if(debuterRoutine){
            prendreAction(instruction);
        }
        positionInitiale++;
    }
}

void prendreAction(uint8_t instruction){

    switch(instruction){
        case 0x01:
        break;

        case 0x44:
        break;
            
    }
}