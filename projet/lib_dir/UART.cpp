#include "UART.h"
#include "includes.h"

UART::UART(){
    initialisationUART();
}

void UART::initialisationUART(){
    
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    // UCSR0A = '';
    // il nest pas necessaire d'ajuster ce registre 
    //Enable Transmission and reception
    UCSR0B |= (1<<RXEN0) | (1 << TXEN0);
    // Format des trames: 8 bits (UCSZ01 et UCSZ00) (3<<UCSZn0) par defaut deja a 1 
    //, 1 stop bits (USBSn = 0), none parity (UPMN) ;
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);

}

//Fonction transmissionUART, prise de la documentation officielle ATMEL
void UART::transmissionUART(uint8_t donnee)
{
    //Wait for empty transmit buffer
    while(!(UCSR0A & (1<< UDRE0))){
    
         UDR0 = donnee;
    }
}

//Fonction reception UART, prise de la documentation officielle ATMEL
uint8_t UART::receptionUART()
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

