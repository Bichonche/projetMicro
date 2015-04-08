#include <stdbool.h>
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_timer.h"
#include "systen_delay.h"


void emitMessage(uint8_t * tabMessage, bool *);

void init(void);
void deInit(void);

void header(void);
void bit_0(void);
void bit_1(void);
void eom(void);

void signal(bool, double);

