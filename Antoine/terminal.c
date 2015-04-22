#include "terminal.h"

uint8_t indice = 0; //Indice du tableau à remplir.
uint8_t Tableau[16]; //Tableau avec les instructions reçues depuis le terminal.
bool finTab;

void initTerminal(void){
	UART_CFG_Type UART_ConfigStruct;
	UART_FIFO_CFG_Type UART_FIFOInitStruct;
	
	// Broche P0.10 en fonction TX2 pour le LCD.
	PINSEL_CFG_Type pinsel = {PINSEL_PORT_0,PINSEL_PIN_10,PINSEL_FUNC_1,PINSEL_PINMODE_PULLUP,PINSEL_PINMODE_NORMAL};
	//Initialisation de la liaison UART.
	UART_ConfigStructInit(&UART_ConfigStruct);
	//Initialisation du FIFO.
	UART_FIFOConfigStructInit(&UART_FIFOInitStruct);
	
	//Configuration des registres.
	UART_Init(LPC_UART2, &UART_ConfigStruct);
	UART_FIFOConfig(LPC_UART2, &UART_FIFOInitStruct);
	PINSEL_ConfigPin(&pinsel);
	
	//Permet le transfert de données.
	UART_TxCmd(LPC_UART2, ENABLE);

	//Déclenche une interruption si des données sont prêtes à être transmises.
	UART_IntConfig(LPC_UART2, UART_INTCFG_RBR, ENABLE);
}

void tabDonnees(uint8_t tableau[16], bool * finTab){
	if(indice < 16){
		tableau[indice] =	UART_ReceiveByte(LPC_UART2);
		indice ++;
	} else {
		*finTab = true;
	}
}

void UART2_IRQHandler(void){
	tabDonnees(Tableau, &finTab);
}
