#include "lpc17xx_pwm.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "systen_delay.h"
#include "lpc17xx_libcfg_default.h"
#include "LPC17xx.h"

// Rayon de la roue
#define R 5
// Distance entre les roues
#define D 20

#define PI 3.1415

void init_PWM ();
void init_Encodeurs ();
void avancer (int speed, int distance);
void rodation (int degre);
void stop ();
