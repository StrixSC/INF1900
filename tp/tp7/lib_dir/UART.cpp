#include "UART.h"
#include "includes.h"

UART::UART(){
    initialisationUART();
}

void UART::initialisationUART(){

    // 2400 bauds. Nous vous donnons la valeur des deux
    // premier registres pour vous éviter des complications
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    UCSR0A = _BV(RXC0) | _BV(TXC0);
    UCSR0B = _BV(RXCIE0) | _BV(TXCIE0) ;
    //Format des trames: 8 bits, 1 stop bits, none parity
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8 Bits, 1 stop bit, with disabled parity.

}

//Fonction transmissionUART, prise de la documentation officielle ATMEL
void UART::transmissionUART(unsigned int data){

    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))

    /* Put data into buffer, sends the data */
    UDR0 = data;
}


//Fonction reception UART, prise de la documentation officielle ATMEL
unsigned int UART::receptionUART(){

    unsigned char resh, resl;
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0))){
        resh = UCSR0B;
        resl = UDR0;
    }
    /* Filter the 9th bit, then return */
    return ((resh << 8) | resl);
}



