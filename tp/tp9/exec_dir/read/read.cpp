#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../lib_dir/bouton.h"
#include "../lib_dir/timer.h"
#include "../lib_dir/del.h"
#include "../lib_dir/can.h"
#include "../lib_dir/memoire_24.h"
#include "../lib_dir/moteur.h"
#include "../lib_dir/UART.h"
 

//Init des objets utiles:
Memoire24CXXX mem;
UART uart; 
Del del;


int main(){ 

    /*
    Methodologie:

    On lit le premier byte.
    On place le premier byte dans une variable 16 bit

    On lit le deuxieme byte
    On shift la variable de 16 bit de 8 vers la gauche et on ajoute le deuxieme byte dedans.

    On ecrit le 16bit dans le internal mem. 
    */

    //On set l'adresse courrante a 0;
    uint8_t adr = 0x00;
    uint16_t current = 0x00;

    //On lit le premier byte de l'adresse
    uint8_t octet_1 = uart.receptionUART();
    
    //On place le premier byte dans le current
    current = octet_1; //Current represente l

    //On lit le second byte
    uint8_t octet_2 = uart.receptionUART();

    //On place le second byte dans le current, apres l'avoir bitshift de 8 position vers la gauche.
    current = (current << 8);
    current |= (octet_2);

    uart.transmissionUART()

    



    return 0;
}   