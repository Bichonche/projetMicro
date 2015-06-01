#include "./include/emissionIR.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_gpio.h"
#define TABLE_LENGTH 260

int it;
bool flagEmit = false;
FunctionalState vt = ENABLE;
PINSEL_CFG_Type PINCFG_0;
TIM_MATCHCFG_Type MATCHCFG_0, MATCHCFG_1;
TIM_TIMERCFG_Type TIMERCFG_0,TIMERCFG_1;
uint32_t tElts[TABLE_LENGTH];
void step(void);

int taille = 16;


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
	TIM_UpdateMatchValue(LPC_TIM0, 0, 14); // @ ~36kHz (34kHz)
		
	
	//MR1
	//MATCHCFG_1.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	MATCHCFG_1.IntOnMatch					= ENABLE;
	MATCHCFG_1.StopOnMatch				= ENABLE;
	
	TIM_ConfigMatch(LPC_TIM1, &MATCHCFG_1);
	
	
	//PORT 1.28
	PINCFG_0.Portnum 	= PINSEL_PORT_1;
	PINCFG_0.Pinnum 	= PINSEL_PIN_28;
	PINCFG_0.Funcnum 	= PINSEL_PORT_3;
	PINCFG_0.Pinmode	= PINSEL_PINMODE_PULLUP;
	PINCFG_0.OpenDrain= PINSEL_PINMODE_NORMAL;
	GPIO_ClearValue(0, 0xFFFFFFFF);
	PINSEL_ConfigPin(&PINCFG_0);
	
	
	//Activer l'interruption
	
	NVIC_EnableIRQ(TIMER1_IRQn);
	
	it = 0;
}

//Envoi d'un tableau de 16 octets, bits a bits
void emitMessage(uint8_t t[16],uint8_t taille) {
	char i, j;
	flagEmit = false;
	//Header
	tElts[0] 							= 90;
	tElts[1] 							= 45;
	//End of message
	tElts[taille*16 + 2] = 6;
  tElts[taille*16 + 3] = 400;
	//Contenu du message
	//On parcourt les 16 emplacements du tableau
	for (i = 0 ; i < taille ; i ++ ) {
		//Puis bit a bit
		for (j = 0 ; j < 8 ; j ++ ) {
				//Dans tous les cas la premiere valeur du doublet est 600us
				tElts[2 + 2 * (i * 8 + j)] 		= 6; 
			//Puis si 1 : 1000us, sinon 2000us
				tElts[2 + 2 * (i * 8 + j) + 1] 	= (t[i] & (1 >> j)) ? 10 : 20;
		}
	}
	step();
}

void step() {
	if (it < TABLE_LENGTH) {
		if (vt == DISABLE) {
			PINCFG_0.Funcnum 	= PINSEL_PORT_0;
		}
		else {
			PINCFG_0.Funcnum 	= PINSEL_PORT_3;
		}
		TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TIMERCFG_1);
		TIM_Cmd(LPC_TIM0, vt);
		vt = (vt == ENABLE) ? DISABLE : ENABLE;
		TIM_UpdateMatchValue(LPC_TIM1, 0, tElts[it]);
		TIM_Cmd(LPC_TIM1, ENABLE);
		it ++;
	}
	else {
	 vt = DISABLE;
	 it = 0;
	 TIM_Cmd(LPC_TIM0, DISABLE);
	 flagEmit = true;
	}
}

void TIMER1_IRQHandler(void) { //Timer1 interrupt handler                     
	TIM_DeInit(LPC_TIM1);
	step();
}

void deInit()
{
	TIM_DeInit(LPC_TIM0);
	TIM_DeInit(LPC_TIM1);
}

//Creation d'un signal
/*void signal(bool enble, uint32_t delay) {
	TIM_UpdateMatchValue(LPC_TIM1, 0, delay);
	if (enble) {
		TIM_Cmd(LPC_TIM0, ENABLE);
	}
	else {
	TIM_Cmd(LPC_TIM0, DISABLE);
	}
}*/

