#include "terminal.h"

uint8_t indice = 0; //Indice du tableau à remplir.
uint8_t Tableau[16] = {0}; //Tableau avec les instructions reçues depuis le terminal. Toutes les cases sont initialisées à 0.
bool finTab = false;

void initTerminal(void){
	UART_CFG_Type UART_ConfigStruct;
	UART_FIFO_CFG_Type UART_FIFOInitStruct;
	
	//Broche P0.2 en fonction TX0 pour la télécommande.
	PINSEL_CFG_Type pinsel = {PINSEL_PORT_0,PINSEL_PIN_2,PINSEL_FUNC_1,PINSEL_PINMODE_PULLUP,PINSEL_PINMODE_NORMAL};
	//Broche P0.3 en fonction RX0 pour la télécommande.
	PINSEL_CFG_Type pinsel2 = {PINSEL_PORT_0,PINSEL_PIN_3,PINSEL_FUNC_1,PINSEL_PINMODE_PULLUP,PINSEL_PINMODE_NORMAL};
	//Initialisation de la structure de configuration de la liaison UART0.
	UART_ConfigStructInit(&UART_ConfigStruct);
	//Initialisation de la structure de configuration du FIFO.
	UART_FIFOConfigStructInit(&UART_FIFOInitStruct);
	
	//Configuration des registres.
	PINSEL_ConfigPin(&pinsel);
	PINSEL_ConfigPin(&pinsel2);
	UART_Init(LPC_UART0, &UART_ConfigStruct);
	UART_FIFOConfig(LPC_UART0, &UART_FIFOInitStruct);
	
	
	//Permet le transfert de données.
	UART_TxCmd(LPC_UART0, ENABLE);

	//Déclenche une interruption si des données sont prêtes à être reçues.
	UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);
	NVIC_EnableIRQ(UART0_IRQn); //Active le système d'interruption.
}

void UART0_IRQHandler(void){
	if(indice < 16 && !finTab){ //Si le tableau n'est pas plein,
		uint8_t carac = UART_ReceiveByte(LPC_UART0); //On traite le caractère envoyé,
		UART_SendByte(LPC_UART0, carac); //Et on l'envoie à l'ordinateur pour l'afficher.
		while(UART_CheckBusy(LPC_UART0)); //On attend que l'envoie soit fait.
		if (carac - '0' >= 0 && carac - '0' <= 10) { //Si le caractère reçu est un entier,
			Tableau[indice] = Tableau[indice] * 10 + (carac - '0'); //On le stocke de manière décimale.
		}
		else{
			if(carac == 0x0D){ //Si le caractère correspond à la touche entrée, on considère la suite d'instructions complète.
				finTab = true; 
				indice = taille(Tableau);
			} else if(carac != ' '){ //On ne prend pas en compte les espaces.
					if(Tableau[indice]!=0) indice++; //Si le tableau n'est pas à sa valeur d'initialisation, on écrit dans la case suivante afin d'éviter l'écrasement. 
					Tableau[indice] = carac;
					indice ++;
				}
		}
	} else { //Sinon, la saisie d'instruction est terminée.
		finTab = true;
		indice = taille(Tableau);
	}
}

int taille(uint8_t tableau[16]){
	int i = 0;
	while(i<16 && tableau[i]!=0){
		i++;
	}
	return i;
}
