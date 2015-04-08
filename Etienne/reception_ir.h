#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"

	
	char marge_haute = 1.2;
	char marge_basse = 0.8;
	
	int valeur_header = 9000;
	int valeur_bit1 = 1000;
	int valeur_bit0 = 2000;
	int valeur_stop = 45000;
	
	int valeur_stop_min = valeur_stop*marge_basse;
	int valeur_stop_max = valeur_stop*marge_haute
	int valeur_bit1_min = valeur_bit1*marge_basse;
	int valeur_bit1_max = valeur_bit1*marge_haute;
	int valeur_bit0_min = valeur_bit0*marge_basse;
	int valeur_bit0_max =  valeur_bit0*marge_haute
	int valeur_header_min = valeur_header*marge_basse;
	int valeur_header_max = valeur_header*marge_haute;

	bool flag_debut_transmission;
	bool flag_fin_transmission;
	bool flag_nouveau_bit;

	bool current_bit;
	bool T[128];	// tableau de 128 bits
	char cpt;	// variable pour compter les bits
	bool flag_fin_enregistrement;
	bool flag_reception_bit;
	
	void enregistrement_message(bool);
	void init(void);
	bool capture_duree(int);

	cpt = 127;



	void init();
	void TIMER0_IRQHandler();
	bool capture_duree(int);
	uint_8_t decodage (bool T[128]);
	
	
