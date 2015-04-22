#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include <stdbool.h>


//Initialise les registres pour utiliser le terminal.
void initTerminal(void);

//Rempli un tableau avec les données transmises par le terminal. Appelée par la routine d'interruption.
void tabDonnees(uint8_t tableau[16], bool * finTab);
