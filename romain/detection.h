#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lpc17xx_pinsel.h"
#include "declarations.h"
#include "lpc17xx_adc.h"
uint16_t chargement (void);
float distance (void);
int arret_obligatoire (float d);
boolean obstacle_cote (void);
int detection (void);
