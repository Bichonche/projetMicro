#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
GPIO_Registres *pGPIO0; // Pointeur sur la structure du GPIO0
GPIO_Registres *pGPIO1; // Pointeur sur la structure du GPIO1
uint16_t chargement (void)
{
uint16_t v;
// On charge les données en entrée sur le GPIO sur le P0.23 dans la valeur v
(PINSEL1) >> 14 = 1;
(PINSEL1) >> 15 = 0;
// On se place en mode AD0.0
// recopie de p0.23 dans v
v = ADC_ChannelGetData(ADC0.0,);
// On renvoie la valeur du voltage en fonction de la luminosité
return v;
}
float distance (void)
{
float d = 0;
uint16 v = chargement; // chargement des données du laser
d = v / 0.26; // inversion de la fonction
return d;
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
void arreturgence (int a)
{
if (a = 2)
then
{
stop();
AfficheEcran(0,0,"reculez");
}
else
{
}
}
boolean obstacle_cote (void)
{
boolean b;
int d1;
int d2;
//chargemet des données des microrupteurs depuis P1.11 et P1.21
d1 = (PGPIO1 -> FIOPIN) >> 8; // copie de p1.11 sur d1 (si 1 le bouton est poussé)
d2 = (PGPIO1 -> FIOPIN) >> 14;// copie de p1.21 sur d2 (si 1 le bouton est poussé)
// Si un des deux boutons est poussé, on envoie un booléen true. Sinon on renvoie false.
if (d1 /= 0) then
{
b = true;
}
else
{
if (d2 /= 0) then
{
b = true;
}
else
{
b = false;
}
}
return b;
}
void detection (void)
{
/*On vient de lancer la detection.*/
/* On mesure la distance et on vérifie qu'aucun obstacle ne se trouve sur le côté*/
float d = distance ();
boolean b = obstacle_cote ();
arreturgence (arret_obligatoire (d));
if (b)
then
{
AfficheEcran(0,0,"Obstacle sur le côté");
}
else
{
}
}
