#include "affichage.h"

void initScreen(void){
	
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
	
	//UART_SendByte(LPC_UART2, 0xA0);
	//wait(10); //Attendre 10 millisecondes pour que l'ecran ait termine de s'allumer.
}

void printScreen(uint8_t * message, uint8_t posX, uint8_t posY){
	//Se positionne pour écrire
	UART_SendByte(LPC_UART2, 0xA1);
	UART_SendByte(LPC_UART2, posX);
	UART_SendByte(LPC_UART2, posY);
	//Ecrit
	UART_Send(LPC_UART2, message, 16, NONE_BLOCKING);
}

void clearScreen(void){
	UART_SendByte(LPC_UART2, 0xA3);
	UART_SendByte(LPC_UART2, 0x01);
}
