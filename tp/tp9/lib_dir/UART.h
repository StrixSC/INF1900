#ifndef UART_H
#define UART_H

class UART {
    public:
        UART();     
        void initialisationUART();
        void transmissionUART(unsigned char data);
        unsigned char receptionUART();
};

#endif