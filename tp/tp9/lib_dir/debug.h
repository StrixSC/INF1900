
#include "UART.h"
#include "string.h"

#ifdef DEBUG
UART uart;
#define DEBUG_PRINT( str);for (uint8_t i=0; i<strlen(str);i++){uart.transmit(str[i]);}  

#else

#define DEBUG_PRINT(str) \
  do {                 \
  } while (0)

#endif