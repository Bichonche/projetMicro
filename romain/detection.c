#include "lpc17xx_pinsel.h"
#include "detection.h"
#include "lpc17xx_adc.h"
#include <stdbool.h>
GPIO_Registres *pGPIO0; // Pointeur sur la structure du GPIO0
GPIO_Registres *pGPIO1; // Pointeur sur la structure du GPIO1
unsigned int FIO1PIN;
bool collision_gauche;
bool collision_droit;

uint16_t chargement (void)
	{
	uint16_t v;
// On charge les données en entrée sur le GPIO sur le P0.23 dans la valeur v
// On se place en mode AD0.0
// recopie de p0.23 dans v
		while(ADC_ChannelGetStatus(LPC_ADC,0,1) != 1)
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

bool obstacle_cote (void)
	{
bool b;
int d1;
int d2;
//chargemet des données des microrupteurs depuis P1.11 et P1.21
d1 = (FIO1PIN >>11) & 0x01; // copie de p1.11 sur d1 (si 1 le bouton est poussé)
d2 = (FIO1PIN >>21) & 0x01;// copie de p1.21 sur d2 (si 1 le bouton est poussé)
// Si un des deux boutons est poussé, on envoie un booléen true. Sinon on renvoie false.

	if (d1)
	{
		EINT0_IRQHandler();
	}
 else
 {
	 if (d2)
	 {
		 EINT1_IRQHandler();
	 }
 }
}

void EINT0_IRQHandler (void)
	{
    collision_gauche = true;
	}

void EINT1_IRQHandler (void)
	{
		collision_droit = true;
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

void detectionglobale (void)
{
	obstacle_cote();
}
