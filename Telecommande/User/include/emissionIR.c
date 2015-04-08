#include "./include/emissionIR.h"

bool flagDebutEnvoi, flagFinEnvoi;


typedef struct message
{
	char 			dir;
	uint8_t 	val;
}typeMessage;
//Envoi d'un message ex : ( A, 30)
void emitMessage(char dir, uint8_t dir, bool flagEMit)
{
	
}


//Initialisation
void init()
{
	//Configuration des timer
	//TIMER0
	TIM_TIMERCFG_Type TIMERCFG_0;
	TIMERCFG_0.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIMERCFG_0.PrescaleValue 	= 100;
	
	//TIMER1
	TIM_TIMERCFG_Type TIMERCFG_1;
	TIMERCFG_1.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TIMERCFG_1.PrescaleValue 	= 100;
	
	//MR0
	TIM_MATCHCFG_Type MATCHCFG_0;
	MATCHCFG_0.MatchChannel		= 0;
	MATCHCFG_0.IntOnMatch			= ENABLE;
	MATCHCFG_0.StopOnMatch		= DISABLE;
	MATCHCFG_0.ResetOnMatch		= DISABLE;
	MATCHCFG_0.ExtMatchOutput	=	TIM_EXTMATCH_TOGGLE;
	MATCHCFG_0.MatchValue			= 14; 
	
	//Initialisation des timer
	//TIMER0 et MR0
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCFG_0);
	TIM_ConfigMatch(LPC_TIM0, &MATCHCFG_0);
	
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

//Generer signal pendant x us
void signal(uint1_t signalValue, uint32_t )
{
	
}


