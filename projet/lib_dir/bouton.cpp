#include "bouton.h"

    Bouton::Bouton(){}

/*
@Brief: destructeur de bouton 
@Param: rien
@Return: rien
*/

    Bouton::~Bouton(){}

/*
@Brief: La fonctione debounce, permet deviter les mauvaises lectures causer par le rebonds des pieces mecaniques du bouton
@Param: rien
@Return: unit8_t
*/

    uint8_t Bouton::debounce(){
        if(PIND & 0x04){
            _delay_ms(10);
            if(PIND & 0x04){
                setClicked(1);
                return 1;
            }
        }
        else{
            setClicked(0);
            return 0;
        }
    }
/*
@Brief: permet de verifier l'etat du bouton. Est-il clicker ou pas 
@Param: rien
@Return: uint8_t clicked
*/


    uint8_t Bouton::getClicked() {
        return clicked;
    }

/*
@Brief: permet de changer letat du bouton selon le parametre
@Param: uint8_t val
@Return: rien
*/


    void Bouton::setClicked(uint8_t val) {
        clicked = val;
    }

/*
@Brief: Permet de voir si le bouton a ete clicker.Utilise la fonction debounce
@Param: aucun
@Return: uint8_t
*/

    uint8_t Bouton::click(){
        if(debounce()){
            return 1;
        }
        else   
            return 0;
    }