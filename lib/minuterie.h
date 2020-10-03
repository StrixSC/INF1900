#ifndef MINUTERIE_H
#define MINUTERIE_H
#include "includes.h"

class Minuterie {
    public:
    Minuterie();
    ~Minuterie();

    void start(uint8_t time);
    void stop();

    void init();
    
};
#endif