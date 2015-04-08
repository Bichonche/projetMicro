#include <stdbool.h>
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_timer.h"
#include "systen_delay.h"


void emitMessage(uint8_t * tabMessage, bool);

void init();
void deInit();

void header();
void bit_0();
void bit_1();
void eof();

void signal(bool, uint8_t);

