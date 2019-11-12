#ifndef UART_H
#define UART_H
#include "includes.h"

class UART {
    public:
        UART();     
        void initialisationUART();
        void transmissionUART(uint8_t data);
        uint8_t receptionUART();
};

#endif