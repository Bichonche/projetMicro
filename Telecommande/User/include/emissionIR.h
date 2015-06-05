#include <stdbool.h>
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_timer.h"
#include "systen_delay.h"

extern bool flagEmit;
extern int taille;

void emitMessage(uint8_t tabMessage[16],uint8_t taille);

void init(void);
void deInit(void);

void header(void);
void bit_0(void);
void bit_1(void);
void footer(void);
void signal_0(uint32_t);
void signal_1(uint32_t);
void signal(uint32_t , uint32_t);

