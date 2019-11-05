#include "../lib_dir/UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "del.h"

int main(){
    //nous créons des variables de type de chacune des classes que nous allons utliser
    UART uart;
    Memoire24CXXX memoire;
    uint16_t position=0x00;
    uint8_t premier_Octet;
    uint8_t second_Octet;
    uint16_t tailleOctet;
    DDRB=0XFF;
    Del del;
    Port port =B;
    del.allumerDEL(port);

    //La mémoire reçoit le premier octet du uart
    premier_Octet = uart.receptionUART();
    
    //La mémoire reçoit le second octet du uart
    second_Octet=uart.receptionUART();

    //écrire le premier bit dans la mémoire
    memoire.ecriture(position, premier_Octet);
    _delay_ms(4); //dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
    position++; //la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée
    
    //écrire le premier bit dans la mémoire
    memoire.ecriture(position, second_Octet);
    _delay_ms(4); //dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
    position++; //la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée

    //Taille des octets pour savoir le nombre de fois que la boucle while va s executer pour ecrire
    premier_Octet=premier_Octet<<4; //decalage de 4 bits permettra de faire un or avec le second octet et ainsi avoir une valeur sur 16 bits qui representera la taille des 2 octets
    tailleOctet = premier_Octet || second_Octet;
    
    //boucle pour recevoir et ecrire les donnees dans la memoire
    while(position < tailleOctet){
        //Pour pouvoir transmettre une donnée en mémoire externe, nous devons d'abord en recevoir une. La donnée provient de uart, donc nous devons la recevoir
        uint8_t transmission = uart.receptionUART(); //transmission est de type uint16_t en raison de la classe memoire_24.h
        memoire.ecriture(position, transmission); //ecrit a la position initiale la donnee qui a été recue a partir de uart
        _delay_ms(4); //dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
        position++;
    }
    del.eteindre(port);
}