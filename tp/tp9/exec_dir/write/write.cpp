/**************************************************************************************************************************
Auteurs: Nawras Mohammed Amin, John Maliha, Johnny Khoury, Fadi Nourredine
Description: Ce fichier permet d'accéder à la mémoire puis de prendre les éléments dans la mémoire interne et les mettre dans la mémoire externe.
Fichiers utilisés: Uart.h, includes.h, memoire_24.h, del.h, enums.h

**************************************************************************************************************************/


#include "../lib_dir/UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/del.h"
#include "../lib_dir/enums.h"

//nous créons des variables de type de chacune des classes que nous allons utliser
Memoire24CXXX mem;
UART uart;
Port port = B;
Del del;

int main(){

    DDRA = 0x00;
    DDRB = 0xFF;
    PORTA = 0x00;
    uint8_t octet1;
    uint8_t octet2;
    uint16_t tailleTotal;
    uint8_t adr = 0x00;

    //Methodologie: On cherche a obtenir la taille total afin d'obtenir de pouvoir obtenir les instructions du debut jusqua la fin.
    //La mémoire reçoit le premier octet du uart
    octet1 = uart.receptionUART();
    tailleTotal = octet1;
    
    //Taille des octets pour savoir le nombre de fois que la boucle while va s executer pour ecrire
    tailleTotal = (tailleTotal << 8);//decalage de 8 bits permettra de faire un or avec le second octet et ainsi avoir une valeur sur 16 bits qui representera la taille des 2 octets
    
    //écrire le premier bit dans la mémoire
    mem.ecriture(adr, octet1);
     _delay_ms(4); //dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bien ecrite
    adr++;//la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée
    
    //La mémoire reçoit le second octet du uart
    octet2 = uart.receptionUART();
    tailleTotal |= octet2;

    mem.ecriture(adr, octet2);
    _delay_ms(4); //dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bien ecrite
    adr++;//la position de la mémoire augmente de 1, car la premiere case vient d etre utilisée

    //On connait maintenant la taille totale du fichier, et nous avons l'avons aussi transmit au fichier binaire. 
    //Il ne reste donc plus qu'a lire chacune des instructions et les transferer vers la memoire interne jusqu'a ce qu'on atteingne la taille totale. 
    //boucle pour recevoir et ecrire les donnees dans la memoire
    while(adr < tailleTotal){
        //Pour pouvoir transmettre une donnée en mémoire externe, nous devons d'abord en recevoir une. La donnée provient de uart, donc nous devons la recevoir
        uint8_t instruction = 0x00;
        instruction = uart.receptionUART();//transmission est de type uint16_t en raison de la classe memoire_24.h
        mem.ecriture(adr, instruction);//ecrit a la position initiale la donnee qui a été recue a partir de uart
        adr++;
        _delay_ms(4);//dans la documentation à la page 26, on parle d'un delai de programmation de 3.3ms lorsqu'on veut ecrire dans la memoire, donc nous fixons le delai a 4ms pour etre sur que la donnee est bine ecrite
    }
}