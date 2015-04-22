#include "reception.h"

void init(void)
{
	
	PINSEL_CFG_Type PinCfgStruct;
	TIM_CAPTURECFG_Type CaptureCfgStruct;

	// initialisation des variables
	flag_debut_transmission = 0;
	flag_fin_transmission = 0;
	flag_fin_enregistrement = 0;
	flag_reception_bit = 0;
	
	// configuration du pinsel
	PinCfgStruct.Portnum = PINSEL_PORT_0;
	PinCfgStruct.Pinnum = PINSEL_PIN_24;
	PinCfgStruct.Funcnum = PINSEL_FUNC_3;			//CAP3[1]
	PinCfgStruct.Pinmode = PINSEL_PINMODE_PULLUP;
	PinCfgStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&PinCfgStruct);
	
	// initialisation du timer en mode capture 
	CaptureCfgStruct.CaptureChannel = 1;		//channel 1
	CaptureCfgStruct.FallingEdge = ENABLE;
	CaptureCfgStruct.IntOnCaption = DISABLE;
	CaptureCfgStruct.RisingEdge = ENABLE;			// sur les deux fronts
	TIM_ConfigCapture(LPC_TIM3, &CaptureCfgStruct);
	
}


/* Interruption du timer ---------------------------------------------------------- */

void TIMER0_IRQHandler()
{
	
}


/* Capture du bit courant ----------------------------------------------------------- */

bool capture_duree(int duree)
{
	
	// définition des intervalles de vérification des valeurs temporelles
	float valeur_stop_min = valeur_stop * marge_basse;
	float valeur_stop_max = marge_haute * valeur_stop;
	float valeur_bit1_min = valeur_bit1 * marge_basse;
	float valeur_bit1_max = valeur_bit1 * marge_haute;
	float valeur_bit0_min = valeur_bit0 * marge_basse;
	float valeur_bit0_max =  valeur_bit0 * marge_haute;
	float valeur_header_min = valeur_header * marge_basse;
	float valeur_header_max = valeur_header * marge_haute;
	
	while (cpt >= 0)
	{
		
		duree = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
		
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
	
	TIM_ResetCounter(LPC_TIM3);
	
	return current_bit;	
	
}

/* Decodage du message ------------------------------------------------------------------- */

unsigned int decodage (bool T[128])
{
	
}
