#include "lpc17xx_clkpwr.h"
#include "lpc17xx_dac.h"


//Envoi d'un message ex : ( A, 30)
void emitMessage(char dir, uint8_t dir)
{
	
}

//Generation de la porteuse : TIMER0
// 	Utilisation de MAT.0
// 	PC 	= 100;
// 	PR 	= 1389;
//	TCR =	0;

//Initialisation
void init()
{
	//Configuration des timer
	//TIMER0
	TIM_TIMERCFG_Type TIMERCFG_0;
	TIMERCFG_0.PrescaleOption = TIM_PRESCALE_USVAL;
	TIMERCFG_0.PrescaleValue 	= 1;
	//TIMER1
	TIM_TIMERCFG_Type TIMERCFG_1;
	TIMERCFG_1.PrescaleOption = TIM_PRESCALE_USVAL;
	TIMERCFG_1.PrescaleValue 	= 1;
	//Initialisation des timer
	//TIMER0
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCFG_0);
	//TIMER1
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIMERCFG_1);
}

void deInit()
{
	TIM_DeInit(LPC_TIM0);
	TIM_DeInit(LPC_TIM1);
}

//Construction du header
void header()
{
	//	9000us à 1
	
	// 	4500us à 0
}
//Construction d'un bit 0
void bit_0()
{
	//	600us à 1
	
	// 	1000us à 0
	
}

//Construction d'un bit 1
void bit_1()
{
	// 	600us à 1
	
	//	2000us à 0
}
//Fin de trame
void eot()
{
	// 600us à 1
	
	// 40000us à 0
}


