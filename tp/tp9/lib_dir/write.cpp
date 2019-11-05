#include "UART.h"
#include "includes.h"
#include "memoire_24.h"



int main(){
    //nous créons des variables de type de chacune des classes que nous allons utliser
    UART uart;
    Memoire24CXXX memoire;
    uint16_t positionInitiale=0x00;
    uint16_t tailleOctet;

    //La mémoire reçoit le premier bit du uart
    tailleOctet = uart.receptionUART();
    
    //écrire le premier bit dans la mémoire
    memoire.ecriture(positionInitiale, tailleOctet);
    _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite

    positionInitiale++;//la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée

    //La mémoire reçoit le deuxieme bit du uart
    tailleOctet = uart.receptionUART();
    
    //écrire le premier bit dans la mémoire
    memoire.ecriture(positionInitiale, tailleOctet);
    _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite

    positionInitiale++;//la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée

    //Taille des octets pour savoir le nombre de fois que la boucle while va s executer pour ecrire
    tailleOctet=tailleOctet& (1<<transmission); //formule prise de la documentation du atmega à la page 27 du document
    
    //boucle pour recevoir et ecrire les donnees dans la memoire
    while(positionInitiale < tailleOctet){
        //Pour pouvoir transmettre une donnée en mémoire externe, nous devons d'abord en recevoir une. La donnée provient de uart, donc nous devons la recevoir
        uint16_t transmission = uart.receptionUART(); //transmission est de type uint16_t en raison de la classe memoire_24.h
        memoire.ecriture(positionInitiale, transmission);//ecrit a la position initiale la donnee qui a été recue a partir de uart
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        positionInitiale++;
    }
}