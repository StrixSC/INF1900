#ifndef UART_H
#define UART_H

class UART {
    UART();     
    void initialisationUART();
    void transmissionUART(unsigned int data);
    unsigned int receptionUART();
};

#endif