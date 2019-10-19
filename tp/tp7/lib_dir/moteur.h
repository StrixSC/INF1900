#pragma once
#ifndef MOTEUR_H
#define MOTEUR_H
#include "includes.h"

class Moteur {
    public:

        Moteur();
        ~Moteur();

        void start();
        void stop();

        void turn();

    private:

};
#endif