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
#include "../lib_dir/piezo.h"
#include "../lib_dir/moteur.h"


//nous créons des variables de type de chacune des classes que nous allons utliser
Del del;
Piezo piezo;
Moteur moteur;

int main(){

    DDRB = 0xFF;
    DDRD = 0xFF;
    PORTB = 0xFF;
    uint8_t i= 45;
        
    while(i <= 81){
        piezo.play(60);
        _delay_ms(100);
        i++;
    }

    for(;;){
        moteur.startEngine();
        moteur.changeSpeed(50,50);
    }
}