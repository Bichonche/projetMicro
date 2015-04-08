#include "reception.h"

void init()
{
	// initialisation des variables
	flag_debut_transmission = 0;
	flag_fin_transmission = 0;
	flag_fin_enregistrement = 0;
	flag_reception_bit = 0;
	
	// configuration du pinsel
	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum = PINSEL_PORT_0;
	PinCfg.Pinnum = PINSEL_PIN_24;
	PinCfg.Funcnum = PINSEL_FUNC_3;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&PinCfg);
	
	// initialisation du timer en mode counter
	TIM_TIMERCFG_Type ConfigStruct;
	TIM_ConfigStructInit(TIM_COUNTER_FALLING_MODE, &ConfigStruct);

	TIM_Init(&LPC_TIM3, TIM_COUNTER_FALLING_MODE, &ConfigStruct);
	
}


/* Interruption du timer ---------------------------------------------------------- */

void TIMER0_IRQHandler()
{
	
}


/* Capture du bit courant ----------------------------------------------------------- */

bool capture_duree(int duree)
{
	
	while (cpt >= 0)
	{
		if (duree > valeur_header_min && duree < valeur_header_max)		// debut de la transmission
		{
			flag_debut_transmission = 1;
		}
		
		else if (duree > valeur_bit1_min && duree < valeur_bit1_max)		// le bit vaut 1
		{
			flag_reception_bit = 1;
			current_bit = 1;
			T[cpt] = current_bit;			// on le stocke dans le tableau
			cpt--;
		}
		
		else if (duree > valeur_bit0_min && duree < valeur_bit0_max)		// le bit vaut 0
		{
			flag_reception_bit = 1;
			current_bit = 0;
			T[cpt] = current_bit;			// on le stocke dans le tableau
			cpt--;
		}
		
		else if (duree > valeur_stop_min && duree < valeur_stop_max)		// fin de la transmission
		{
			flag_fin_transmission = 1;
		}
		
		else		// il ne se passe rien -> on se cale à nouveau 
		{
			init();
		}
	}
	return current_bit;	
	
}

/* Decodage du message ------------------------------------------------------------------- */

uint_8_t decodage (bool T[128])
{
	
}
