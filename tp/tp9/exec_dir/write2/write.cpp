#include "../lib_dir/UART.h"
#include "../lib_dir/includes.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/del.h"
#include "../lib_dir/enums.h"

//variables necessaires:
Memoire24CXXX mem;
UART uart;
Port port = B;
Del del;

int main(){

    DDRA = 0x00;
    DDRB = 0xFF;
    
    // uint8_t deuxPremiersOctets[] = {0x00, 0x00};
    uint8_t octet1;
    uint8_t octet2;
    
    uint16_t tailleTotal;
    uint8_t adr = 0x00;

    //Methodologie: On cherche a obtenir la taille total afin d'obtenir de pouvoir obtenir les instructions du debut jusqua la fin.

    octet1 = uart.receptionUART();
    tailleTotal = octet1;
    tailleTotal = (tailleTotal << 8);

    mem.ecriture(adr, octet1);
    adr++;

    octet2 = uart.receptionUART();
    tailleTotal |= octet2;

    mem.ecriture(adr, octet2);
    adr++;

    //On connait maintenant la taille totale du fichier, et nous avons l'avons aussi transmit au fichier binaire. 
    //Il ne reste donc plus qu'a lire chacune des instructions et les transferer vers la memoire interne jusqu'a ce qu'on atteingne la taille totale. 

    while(adr < tailleTotal){
        uint8_t instruction = 0x00;
        instruction = uart.receptionUART();
        mem.ecriture(adr, instruction);
        adr++;
        _delay_ms(4);

        // uint8_t operande = 0x00;
        // operande = uart.receptionUART();
        // mem.ecriture(adr, operande);
        // adr++; 
        // _delay_ms(4);
    }
}