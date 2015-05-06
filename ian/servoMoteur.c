#include "servoMoteur.h"
//#include "detection.h"

void init_PWM ()
{
	PWM_TIMERCFG_Type PWM_struct;
	PINSEL_CFG_Type PinCfg;
	PWM_MATCHCFG_Type PWM_MatchConfigStruct;
	
	// Mode PWM de P2.1 et P2.3
	PinCfg.Portnum = PINSEL_PORT_2; 
	PinCfg.Pinnum = PINSEL_PIN_1; 
	PinCfg.Funcnum = PINSEL_FUNC_1; 
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP; 
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL; 
	PINSEL_ConfigPin(&PinCfg); // Config de P2.1
	PinCfg.Pinnum = PINSEL_PIN_3;
	PINSEL_ConfigPin(&PinCfg); // Config de P2.3
	
	// Mode Timer de PWM1.2 et PWM1.4
	PWM_ConfigStructInit(PWM_MODE_TIMER,&PWM_struct);
	PWM_Init(LPC_PWM1,PWM_MODE_TIMER,&PWM_struct); // PWM1 en mode Timer
	
	/* Config Timer */
	
	// Prescale
	LPC_PWM1->PR = 25; // TC s'incrémente à chaque µs (PWM_Clock = Clock/4 = 25MHz)
	
	// Config MR0
	PWM_MatchConfigStruct.MatchChannel = 0;
	PWM_MatchConfigStruct.ResetOnMatch = ENABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWM_MatchConfigStruct); // Reset on MR0
	PWM_MatchUpdate(LPC_PWM1, 0, 20000, PWM_MATCH_UPDATE_NOW); // MR0 = 20000 (20ms)

	// Config MR1/MR3
	PWM_MatchUpdate(LPC_PWM1, 1, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, 3, 0, PWM_MATCH_UPDATE_NOW);
		
	// Config MR2/MR4 (arrêt)
	PWM_MatchUpdate(LPC_PWM1, 2, 1.5*1000, PWM_MATCH_UPDATE_NOW); // Moteur gauche
	PWM_MatchUpdate(LPC_PWM1, 4, 1.5*1000, PWM_MATCH_UPDATE_NOW); // Moteur droit 
	
	// PWM1.2 et PWM1.4 ENABLE
	PWM_ChannelCmd(LPC_PWM1, 2, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, 4, ENABLE);
	
	LPC_PWM1->TCR |= 1<<3; // PWM Enable

}

void init_Encodeurs () 
{
	PINSEL_CFG_Type PinCfg;
	TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
	
	// Mode CAP de P1.18 et P1.26
	PinCfg.Portnum = PINSEL_PORT_1; 
	PinCfg.Pinnum = PINSEL_PIN_18; 
	PinCfg.Funcnum = PINSEL_FUNC_3; 
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP; 
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL; 
	PINSEL_ConfigPin(&PinCfg); // Mode CAP1[0] sur P1.18
	PinCfg.Pinnum = PINSEL_PIN_26;
	PINSEL_ConfigPin(&PinCfg); // Mode CAP0[0] sur P1.26
	
	// Timer en compteur any edge
	LPC_TIM0->CTCR = 3;
	LPC_TIM1->CTCR = 3;
	
	// Mode Capture de CAP1[0] et CAP0[0]
	TIM_CaptureConfigStruct.CaptureChannel = 0;
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	TIM_CaptureConfigStruct.FallingEdge = ENABLE;
	TIM_CaptureConfigStruct.IntOnCaption = DISABLE;
	TIM_ConfigCapture(LPC_TIM0, &TIM_CaptureConfigStruct); // TIM0 en mode capture 
	TIM_ConfigCapture(LPC_TIM1, &TIM_CaptureConfigStruct); // TIM1 en mode capture 
	
}

void avancer (int speed, int distance)
{
	float perim = 2*PI*R;
	
	// Config MR2/MR4 pour Avancer/Reculer
	PWM_MatchUpdate(LPC_PWM1, 2, (1.5+0.5*speed/100)*1000, PWM_MATCH_UPDATE_NOW); // Moteur gauche
	PWM_MatchUpdate(LPC_PWM1, 4, (1.5-0.5*speed/100)*1000, PWM_MATCH_UPDATE_NOW); // Moteur droit	
	
	// Mise en route des encodeurs
	TIM_Cmd(LPC_TIM0,ENABLE); // Capture CAP0[0] Enable
	TIM_Cmd(LPC_TIM1,ENABLE); // Capture CAP1[0] Enable
	// Mise en route des moteurs
	LPC_PWM1->TCR |= 1; // Counter Enable	
	
	// Attente du parcourt de la distance souhaitée ou de l'apparition d'un obstacle
	while ((TIM_GetCaptureValue(LPC_TIM0,TIM_COUNTER_INCAP0)<88*distance/perim || TIM_GetCaptureValue(LPC_TIM1,TIM_COUNTER_INCAP0)<88*distance/perim) && detection() != 2)
	{
		if (detection() == 1) // On est à moins de 30cm d'un obstacle donc on ralentit
		{
			PWM_MatchUpdate(LPC_PWM1, 2, (1.5+0.5*speed/200)*1000, PWM_MATCH_UPDATE_NOW); // Moteur gauche à speed/2
			PWM_MatchUpdate(LPC_PWM1, 4, (1.5-0.5*speed/200)*1000, PWM_MATCH_UPDATE_NOW); // Moteur droit à speed/2
		}
	}
	// Arrêt du robot
	stop();

}

void rotation (int degre) // degre dans [-180;180]
{
	float nb_fronts;
	
	if (degre > 0) { // Tourner à gauche
		
		// Config MR2/MR4 pour Tourner
		PWM_MatchUpdate(LPC_PWM1, 2, 1000, PWM_MATCH_UPDATE_NOW); // Moteur gauche (vitesse max en marche arrière)
		PWM_MatchUpdate(LPC_PWM1, 4, 1000, PWM_MATCH_UPDATE_NOW); // Moteur droit (vitesse max en marche arrière)
		
		nb_fronts = 88*2*R/D*degre/360;
		
	} else { // Tourner à droite
		
		// Config MR2/MR4 pour Tourner
		PWM_MatchUpdate(LPC_PWM1, 2, 2000, PWM_MATCH_UPDATE_NOW); // Moteur gauche (vitesse max en marche avant)
		PWM_MatchUpdate(LPC_PWM1, 4, 2000, PWM_MATCH_UPDATE_NOW); // Moteur droit (vitesse max en marche avant)
		
		nb_fronts = -88*2*R/D*degre/360;
	}
	
	// Mise en route des encodeurs
	TIM_Cmd(LPC_TIM0,ENABLE); // Capture CAP0[0] Enable
	TIM_Cmd(LPC_TIM1,ENABLE); // Capture CAP1[0] Enable
	// Mise en route des moteurs
	LPC_PWM1->TCR |= 1; // Counter Enable
	
	// Attente du parcourt de la distance souhaitée
	while (TIM_GetCaptureValue(LPC_TIM0,TIM_COUNTER_INCAP0)<nb_fronts || TIM_GetCaptureValue(LPC_TIM1,TIM_COUNTER_INCAP0)<nb_fronts) 
	{}
	// Arrêt du robot
	stop();
}

void stop ()
{ 
	/* Gestion des servoSmoteurs */
	
	// Arrêt du PWM
	LPC_PWM1->TCR &= ~1; // Counter Disable
	// RAZ des PWM 
	LPC_PWM1->TCR |= 1<<1; // Counter Reset
	LPC_PWM1->TCR &= ~(1<<1); // Clear Reset
	
	// Par sécurité on met les config d'arrêt pour les deux moteurs
	PWM_MatchUpdate(LPC_PWM1, 2, 1.5*1000, PWM_MATCH_UPDATE_NOW); // Moteur gauche à l'arrêt
	PWM_MatchUpdate(LPC_PWM1, 4, 1.5*1000, PWM_MATCH_UPDATE_NOW); // Moteur droit à l'arrêt
	
	/* Gestion des encodeurs */
	
	// Arrêt des Timers
	TIM_Cmd(LPC_TIM0,DISABLE); // Capture CAP0[0] Disable
	TIM_Cmd(LPC_TIM1,DISABLE); // Capture CAP1[0] Disable
	// RAZ des registres de capture CR0
	TIM_ResetCounter(LPC_TIM0);
	TIM_ResetCounter(LPC_TIM1);

}

