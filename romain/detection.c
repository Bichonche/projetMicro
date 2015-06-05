#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "detection.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include <stdbool.h>
GPIO_Registres *pGPIO0; // Pointeur sur la structure du GPIO0
GPIO_Registres *pGPIO1; // Pointeur sur la structure du GPIO1

unsigned int FIO1PIN;
bool collision_gauche;
bool collision_droit;
int d1;
int d2;

uint16_t chargement (void)
{
uint16_t v;
// On charge les données en entrée sur le GPIO sur le P0.23 dans la valeur v
// On se place en mode AD0.0
// recopie de p0.23 dans v
while(!(ADC_ChannelGetStatus(LPC_ADC,0, 1 )))
{
v = ADC_ChannelGetData(LPC_ADC,0);
}
// On renvoie la valeur du voltage en fonction de la luminosité
return v;
}

float distance (void)
{
float d = 0;
uint16_t v = chargement(); // chargement des données du laser
d = 0.26 / v; // inversion de la fonction
return d;
}

void init (void)
{
	 ADC_Init(LPC_ADC,50);
	 ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,SET);
	 EXTI_Init();
	 EXTI_SetPolarity(EXTI_EINT0, EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE);
	 EXTI_SetPolarity(EXTI_EINT1, EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE);
	 EXTI_Config(EXTI_EINT0);
	 EXTI_Config(EXTI_EINT1);
	 CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD ,ENABLE);
d1 = (FIO1PIN >>11) & 0x01; // copie de p1.11 sur d1 (si 1 le bouton est poussé);
d2 = (FIO1PIN >>12) & 0x01;
}

void EINT0_IRQHandler (void)
{
if (d1)
{
collision_gauche = true;
}  
}

void EINT1_IRQHandler (void)
{
if (d2)
{
collision_droit = true;
}
}
int arret_obligatoire (float d)
{
int a = 0;
if (d < 15)
{
a = 2; // On est trop pres. On doit s'arrêter
}
else
{
if (d > 30)
{
a = 1; // On commence à être trop pres
}
else
{
a = 0; // On est loin d'un obstacle (sur le front du robot)
}
}
return a;
}

int detection (void)
{
/*On vient de lancer la detection.*/
/* On mesure la distance et on vérifie qu'aucun obstacle ne se trouve sur le côté*/
float d = distance ();
return arret_obligatoire(d);
}
