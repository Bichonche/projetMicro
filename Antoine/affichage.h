#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

//Initalise l'écran et la liaison UART, prend 20 ms.
void initScreen(void);

//Affiche une chaîne de caractère à la position voulue.
void printScreen(uint8_t * message, uint8_t posX, uint8_t posY);

//Efface l'écran.
void clearScreen(void);
