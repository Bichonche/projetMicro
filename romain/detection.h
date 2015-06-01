#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lpc17xx_pinsel.h"
#include "declarations.h"
#include "lpc17xx_adc.h"
uint16_t chargement (void);
float distance (void);
bool obstacle_cote (void);
int detection (void);
void EINT0_IRQHandler (void);
void EINT1_IRQHandler (void);
