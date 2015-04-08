#include "./include/emissionIR.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"

bool enble;
PINSEL_CFG_Type PINCFG_0;
TIM_MATCHCFG_Type MATCHCFG_0;
TIM_TIMERCFG_Type TIMERCFG_0,TIMERCFG_1;

//Envoi d'un message ex : ( A, 30)
void emitMessage(uint8_t * t, bool  * flagEmit)
{
	char i, j;
	header();
	for (i = 0; i < 16; i ++ )
		for (j = 0; j < 8; j ++ )
			if (t[i] & (1 >> j))
				bit_1();
			else
				bit_0();
	eom();
	*flagEmit = true;
}
//Initialisation
void init()
{
	//Initialisation des délais en ms
	delay_init();
	//Configuration des timer
	//TIMER0
	TIMERCFG_0.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIMERCFG_0.PrescaleValue 	= 100;
	
	//TIMER1
	TIMERCFG_1.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIMERCFG_1.PrescaleValue 	= 100;
	
	//MR0

	MATCHCFG_0.MatchChannel				= 0;
	MATCHCFG_0.IntOnMatch					= ENABLE;
	MATCHCFG_0.StopOnMatch				= DISABLE;
	MATCHCFG_0.ResetOnMatch				= DISABLE;
	MATCHCFG_0.ExtMatchOutputType	=	TIM_EXTMATCH_TOGGLE;
	MATCHCFG_0.MatchValue					= 14; 
	
	//Config du port Emetteur IR

	PINCFG_0.Portnum 	= 1;
	PINCFG_0.Pinnum 	= 28;
	PINCFG_0.Funcnum 	= 3;
	PINCFG_0.Pinmode	= PINSEL_PINMODE_PULLUP;
	PINCFG_0.OpenDrain= PINSEL_PINMODE_NORMAL;
	
	PINSEL_ConfigPin(&PINCFG_0);
	//Initialisation des timer
	//TIMER0 et MR0
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCFG_0);
	TIM_ConfigMatch(LPC_TIM0, &MATCHCFG_0);
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
	signal(true, 0.6);
	// 	4500us à 0
	signal(false, 4.5);
}
//Construction d'un bit 0
void bit_0()
{
	//	600us à 1
	signal(true, 0.6);
	// 	1000us à 0
	signal(false, 1);
	
}

//Construction d'un bit 1
void bit_1()
{
	// 	600us à 1
	signal(true, 0.6);
	//	2000us à 0
	signal(false, 2);
}
//Fin de trame
void eom()
{
	// 600us à 1
	signal(true, 0.6);
	// 40000us à 0
	signal(false, 40);
}

void signal(bool enble, double length)
{
	if (enble)
		TIM_Cmd(LPC_TIM0, ENABLE);
	else
		TIM_Cmd(LPC_TIM0, DISABLE);
	delay_ms(length);	
}

