
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include <stdbool.h>
	
	float head_min;
	float head_max;
	float bit0_min;
	float bit0_max;
	float bit1_min;
	float bit1_max;
	float stop_min;
	float stop_max;
	
	float marge_haute = 1.2;
	float marge_basse = 0.8;
	unsigned short int head = 9000;
	unsigned short int bit0 = 2000;
	unsigned short int bit1 = 1000;
	unsigned short int stop = 45000;
	
	int cpt_bit;
	int cpt_oct;
	uint32_t duree;
	int Tab[16];
	
	bool flag_debut_reception;
	bool flag_fin_reception;

	void init(void);
	void decodage(void);
	void TIMER3_IRQHandler(void);
	
