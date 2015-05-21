
#include "reception.h"

void init(void)
{
	
	PINSEL_CFG_Type PinCfgStruct;
	TIM_TIMERCFG_Type TimerCfgStruct;
	TIM_CAPTURECFG_Type CaptureCfgStruct;
	
	flag_debut_reception = 0;
	flag_fin_reception = 0;
	cpt_bit = 7;
	cpt_oct = 0;
	
	PinCfgStruct.Portnum = PINSEL_PORT_0;													// Port 0.24
	PinCfgStruct.Pinnum = PINSEL_PIN_24;													// Port 0.24
	PinCfgStruct.Funcnum = PINSEL_FUNC_3;													// CAP3[1]
	PINSEL_ConfigPin(&PinCfgStruct);															// Configuration terminée
	
	TimerCfgStruct.PrescaleOption = TIM_PRESCALE_USVAL; 					// microsec
	TimerCfgStruct.PrescaleValue = 1;															// echelle : 1 µs
	CaptureCfgStruct.CaptureChannel = 1;
	CaptureCfgStruct.FallingEdge = ENABLE;
	CaptureCfgStruct.RisingEdge = ENABLE;
	CaptureCfgStruct.IntOnCaption = ENABLE;
	// CounterCfgStruct.CounterOption = TIM_COUNTER_INCAP1;
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TimerCfgStruct);					// initialisation du timer
	TIM_ConfigCapture(LPC_TIM3, &CaptureCfgStruct);
	
	NVIC_EnableIRQ(TIMER3_IRQn);
	TIM_Cmd(LPC_TIM3, ENABLE);
	
}


void TIMER3_IRQHandler(void)
{
	
	float head_min = head * marge_basse;
	float head_max = head * marge_haute;
	float bit0_min = bit0 * marge_basse;
	float bit0_max = bit0 * marge_haute;
	float bit1_min = bit1 * marge_basse;
	float bit1_max = bit1 * marge_haute;
	float stop_min = stop * marge_basse;
	float stop_max = stop * marge_haute;

	duree = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);				// on récup la valeur du timer
	
	if (cpt_bit < 0)
	{
		cpt_oct++;
		cpt_bit = 7;
	}
	
	if (duree > head_min && duree < head_max)					// debut de la transmission
	{
		flag_debut_reception = 1;														// la réception a commencé
	}
		
	else if (duree > bit1_min && duree < bit1_max)		// le bit vaut 1
	{
		Tab[cpt_oct] |= 1 << cpt_bit;																					// on met un 1 dans le tableau
		cpt_bit--;																							// décrémentation du compteur
	}
		
	else if (duree > bit0_min && duree < bit0_max)		// le bit vaut 0
	{
		Tab[cpt_oct] &= ~(1 << cpt_bit);																					// on met un 0 dans le tableau
		cpt_bit--;																							// décrémentation du compteur
	}
	
	else
	{
		flag_debut_reception = 0;														// on remet le flag de début à 0
		flag_fin_reception = 1;															// on passe le flag de fin à 1 (pour lancer décodage)
	}
	
	TIM_ResetCounter(LPC_TIM3);
	TIM_ClearIntPending(LPC_TIM3, TIM_CR1_INT);
	
}


/* Decodage du message ------------------------------------------------------------------- */

void decodage (void)
{
	

}
