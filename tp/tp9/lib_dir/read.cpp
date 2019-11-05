#include "UART.h"
#include "includes.h"
#include "memoire_24.h"



int main(){
    //nous créons des variables de type de chacune des classes que nous allons utliser
    UART uart;
    Memoire24CXXX memoire;
    uint16_t positionInitiale=0x00;
    uint16_t tailleOctet;

    //Nous devons lire les 2 premiers bits de la mémoire, car ce sont ces bits que nous avons écrits dans la mémoire externe
    for(int i=0; i<2; i++){
        memoire.lecture(positionInitiale, &tailleOctet);
        positionInitiale++;
    }

    //Taille des octets pour savoir le nombre de fois que la boucle while va s executer pour ecrire
    tailleOctet=tailleOctet& (1<<transmission); //formule prise de la documentation du atmega à la page 27 du document
    
    //boucle pour recevoir et ecrire les donnees dans la memoire
    while(positionInitiale < tailleOctet){
        //Pour pouvoir transmettre une donnée en mémoire externe, nous devons d'abord en recevoir une. La donnée provient de uart, donc nous devons la recevoir
        memoire.lecture(positionInitiale, &transmission);//lit a la position initiale la donnee qui a été recue a partir de uart
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        positionInitiale++;
    }
}