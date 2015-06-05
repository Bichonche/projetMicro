#include "./include/emissionIR.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_gpio.h"
#define TABLE_LENGTH 260

int it, check;
int delay;
bool flagEmit = false;
bool flagBitEmit = false;
FunctionalState vt = ENABLE;
PINSEL_CFG_Type PINCFG_0;
TIM_MATCHCFG_Type MATCHCFG_0, MATCHCFG_1;
TIM_TIMERCFG_Type TIMERCFG_0,TIMERCFG_1;
uint32_t tElts[TABLE_LENGTH];
void send(FunctionalState, int);
int taille = 4;


//Initialisation
void init() {
	//Configuration des timer
	//TIMER0 - Porteuse
	TIM_ConfigStructInit(TIM_TIMER_MODE, &TIMERCFG_0);
	TIMERCFG_0.PrescaleOption = TIM_PRESCALE_TICKVAL;
	//TIMERCFG_0.PrescaleValue 	= 99; //Obtention d'un signal @ 1MHz
	
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCFG_0);
	
	
	//TIMER1 - Chrono, en 100us
	TIM_ConfigStructInit(TIM_TIMER_MODE, &TIMERCFG_1);
	TIMERCFG_1.PrescaleOption = TIM_PRESCALE_USVAL;
	TIMERCFG_1.PrescaleValue 	= 99; //100-1
	
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIMERCFG_1);
	
	
	//MR0
	MATCHCFG_0.ExtMatchOutputType	=	TIM_EXTMATCH_TOGGLE;
	MATCHCFG_0.ResetOnMatch				= ENABLE;
	
	TIM_ConfigMatch(LPC_TIM0, &MATCHCFG_0);
	TIM_UpdateMatchValue(LPC_TIM0, 0, 349); // @ ~36kHz
		
	
	//MR1
	//MATCHCFG_1.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	MATCHCFG_1.IntOnMatch					= ENABLE;
	MATCHCFG_1.ResetOnMatch				= ENABLE;
	
	TIM_ConfigMatch(LPC_TIM1, &MATCHCFG_1);
	
	
	//PORT 1.28
	PINCFG_0.Portnum 	= PINSEL_PORT_1;
	PINCFG_0.Pinnum 	= PINSEL_PIN_28;
	PINCFG_0.Funcnum 	= PINSEL_FUNC_3;
	//PINCFG_0.Pinmode	= PINSEL_PINMODE_PULLUP;
	//PINCFG_0.OpenDrain= PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&PINCFG_0);
	
	
	//Activer l'interruption
	
	NVIC_EnableIRQ(TIMER1_IRQn);
	
}

//Envoi d'un tableau de 16 octets, bits a bits
void emitMessage(uint8_t t[16],uint8_t taille) {
	char i, j;
	init();
	flagEmit = false;
	header();
	for (i = 0 ; i < taille ; i ++ ) {
		//Puis bit a bit
		for (j = 0 ; j < 8 ; j ++ ) {
			//Dans tous les cas la premiere valeur du doublet est 600us
			signal_1(6);
			delay = (t[i] & (1 >> j)) ? 10 : 20;
			signal_0(delay);
			}
	}
	footer();
	deInit();
	flagEmit= true;
}

void TIMER1_IRQHandler(void) { 
	//Timer1 interrupt handler                     
	TIM_DeInit(LPC_TIM1);
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIMERCFG_1);
	TIM_DeInit (LPC_TIM0);
	TIM_ResetCounter(LPC_TIM0);
	flagBitEmit = true;
}

void deInit()
{
	TIM_DeInit(LPC_TIM0);
	TIM_DeInit(LPC_TIM1);
}

//Creation d'un signal

void signal_0(uint32_t delay) {
	{
		PINCFG_0.Funcnum = 0;
		PINSEL_ConfigPin(&PINCFG_0);
		GPIO_SetDir(1, (1<<28), 1);
		GPIO_ClearValue(1, (0<<28));
		TIM_UpdateMatchValue(LPC_TIM1, 0, delay);
		TIM_Cmd(LPC_TIM1, ENABLE);
	}while(!flagBitEmit);
	flagBitEmit = false;
}

void signal_1(uint32_t delay) {
	{
		PINCFG_0.Funcnum= 3;
		PINSEL_ConfigPin(&PINCFG_0);
		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCFG_0);
		TIM_Cmd(LPC_TIM0, ENABLE);
		TIM_UpdateMatchValue(LPC_TIM1, 0, delay);
		TIM_Cmd(LPC_TIM1, ENABLE);
	}while(!flagBitEmit);
	flagBitEmit = false;
}
 void header() {
	signal_1(90);
	signal_0(45);
 }
 
 void footer() {
	signal_1(6);
	signal_0(400);
 }
