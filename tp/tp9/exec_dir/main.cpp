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

////////////////////////////////////////////////////////////////
//Liste des instructions en OPCode
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

////////////////////////////////////////////////////////////////
//Variables et initiation d'objets necessaire pour le programme
Memoire24CXXX mem;
UART uart;
Del del;
Moteur moteur;
Piezo piezo;

///////////////////////////////////////////////////////////////
//Variables pour l'instruction de boucle
uint8_t loopAdrStart = 0x00;        //Permet de garder en memoire l'adresse ou la boucle debute
uint8_t loopCounter = 0x00;         //Permet de donner un counter du nombre de repetition pour la boucle

///////////////////////////////////////////////////////////////
//Variable boolean nous permettant de savoir si l'instruction DBT a ete lue
uint8_t debuterRoutine = 0;         

//Variable du current adresse. 
uint16_t adr = 0x00;

///////////////////////////////////////////////////////////////
//Definition des fonctions prendreAction et seanceInit
void prendreAction(uint8_t instruction, uint8_t operande);
void seanceInit();

///////////////////////////////////////////////////////
///////////                                 ///////////
///////////               MAIN              ///////////
///////////                                 ///////////
///////////////////////////////////////////////////////
/*
@Brief: Commence par lire le nombre d'octet du fichier.
Ensuite init une boucle qui lira toute les instruction et leurs operandes
Ce qui permet ensuite l'utilisation de prendreAction() des que l'instruction
de debut est lue
@param: void;
@return: int;
*/
int main(){
    //nous définissons nos entrées et nos sorties
    DDRA = 0x00; //mode entrée
    DDRB = 0xFF; //mode sortie
    DDRD = 0xFF;
    DDRC = 0x00;

    uint16_t tailleTotal = 0x0000;
    uint8_t octet1; ///8 bit, car nous avons besoin d'un octet
    uint8_t octet2; 
    uint8_t counterSong = 45; 

    while (counterSong <= 81){
        piezo.play(counterSong);
        _delay_ms(50);
        counterSong++;
    }

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

/*
@Brief: Cette fonction permet de prendre l'octet representant l'instruction 
ainsi que l'operande, et, grace a une machine a etat, permet d'effectuer l'instruction
a travers le robot.
@Param: uint8_t et uint8_t (deux octets representant l'instruction et l'operande)
@return: void.
*/
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
            del.vert(); //On allume la DEl, peut importe la couleur
        break;

        case DET:
            del.eteindre();
        break;

        case SGO:
            //L'operande represente la note qu'il faut jouer.
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
            moteur.changeSpeed((operande/255.0*100),(operande/255.0*100));
        break;

        case MRE:  //Reculer
            moteur.changeSpeed(-(operande/255.0*100),-(operande/255.0*100));
        break;

        case TRG:  //Tourner Gauche
            moteur.changeSpeed(0,(operande/255.0*100));
        break;

        case TRD:  //Tourner Droite
            moteur.changeSpeed((operande/255.0*100),0);
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
        break;
    }
}

/*
@Brief: Cette fonction ne sert qu'a faire clignoter la del en rouge 2 fois et 
en vert deux fois, pour signifier le debut de la routine.
@param: void;
@return void;
*/
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