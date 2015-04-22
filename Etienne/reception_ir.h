#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include <stdbool.h>
	
	// ---------------- déclarations des constantes & variables globales -------------
	
	// IR peu fiable => on prend +/- 20% du temps demandé
	float marge_haute = 1.2;
	float marge_basse = 0.8;
	
	// temps caractéristiques qui permettent de distinguer les signaux
	unsigned int valeur_header = 9000;
	unsigned int valeur_bit1 = 1000;
	unsigned int valeur_bit0 = 2000;
	int valeur_stop = 45000;
	

	// déclaration des flags
	bool flag_debut_transmission;
	bool flag_fin_transmission;
	bool flag_nouveau_bit;

	// pour la fonction de capture du bit
	bool current_bit;
	bool T[128];
	char cpt = 127;
	bool flag_fin_enregistrement;
	bool flag_reception_bit;
	
	// prototypes des fonctions & procédures
	void enregistrement_message(bool);
	void init(void);
	bool capture_duree(int);
	void TIMER0_IRQHandler(void);
	unsigned int decodage (bool T[128]);
	
	
