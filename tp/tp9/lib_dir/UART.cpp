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

    UCSR0A = 0;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   //Enable transmission & reception
    //Format des trames: 8 bits, 1 stop bits, none parity
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8 Bits, 1 stop bit, with disabled parity.

}

//Fonction transmissionUART, prise de la documentation officielle ATMEL
void UART::transmissionUART(uint8_t data){

    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

//Fonction reception UART, prise de la documentation officielle ATMEL
uint8_t UART::receptionUART()
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

