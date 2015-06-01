#include "lpc17xx_pwm.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "systen_delay.h"
#include "lpc17xx_libcfg_default.h"
#include "LPC17xx.h"

// Rayon de la roue
#define R 3.7
// Distance entre les roues
#define D 16
// Nombre de fronts par tour de roue
#define FRONTS_PAR_TOUR 88

#define VIT_MAX_AVANT 2000
#define VIT_MAX_ARRIERE 1000
#define VIT_NULLE 1500

#define PI 3.1415

void init_PWM (void);
void init_Encodeurs (void);
void avancer (int speed, int distance);
void rotation (int degre);
void stop (void);
