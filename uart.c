#include <lpc214x.h>
void uartInit(void)
{
 //configurations to use serial port
PINSEL0 |= 0x05; // P0.0 & P0.1 ARE CONFIGURED AS TXD0 & RXD0
U0LCR = 0x83; /* 8 bits, no Parity, 1 Stop bit */
U0DLM = 0; 
U0DLL = 8; // 115200 baud rate
U0LCR = 0x03; /* DLAB = 0 */
U0FCR = 0x07; /* Enable and reset TX and RX FIFO. */
}