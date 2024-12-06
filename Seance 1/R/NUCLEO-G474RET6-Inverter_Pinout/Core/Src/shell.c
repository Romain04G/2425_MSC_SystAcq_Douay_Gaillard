/**
 * @file shell.c
 * @brief Fichier contenant le fonctionnement du shell.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 5 decembre 2024
 */

/* Includes ------------------------------------------------------------------*/
#include "shell.h"

/* Variables externes --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Variables globales --------------------------------------------------------*/
char cmd[CMD_BUFFER_SIZE]; //contenant la commande en cours
int idxCmd; //contenant l'index du prochain caractère à remplir
const uint8_t prompt[]="user@Nucleo-STM32G431>>";
const uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G431 |"
		"\r\n*-----------------------------*"
		"\r\n";;
const uint8_t newLine[]="\r\n";
const uint8_t help[] = "Available commands:\r\n"
		"1. help    : Display available commands\r\n"
		"2. pinout  : Show connected pins and their functions\r\n"
		"3. start   : Turn on motor power stage\r\n"
		"4. stop    : Turn off motor power stage\r\n"
		"5. speed   : Modify the motor speed (enter a duty cycle)\r\n"
		"6. current : Give the current value in the motor\r\n"
		"7. vitesse : Give the rotation speed of the motor";
const uint8_t pinout[] = "Pinout information:\r\n"
		"1. PA5  : GPIO Output (LED control)\r\n"
		"2. PA0  : ADC Input (Sensor)\r\n"
		"3. USART2 : UART Communication\r\n";
const uint8_t powerOn[]="Power ON\r\n";
const uint8_t powerOff[]="Power OFF\r\n";
const uint8_t cmdNotFound[]="Command not found\r\n";
const uint8_t wrongargument[]="You need 2 arguments\r\n";
const uint8_t wrongvalue[]="argument need to be bethween 1 and 99 \r\n";

uint32_t 	uartRxReceived; 					//flag de récéption d'un caractère sur la liaison uart
uint8_t 	uartRxBuffer[UART_RX_BUFFER_SIZE]; 	//buffer de réception de donnée de l'uart
uint8_t 	uartTxBuffer[UART_TX_BUFFER_SIZE]; 	//buffer d'émission des données de l'uart

char	 	cmdBuffer[CMD_BUFFER_SIZE];
int 		idx_cmd;
char* 		argv[MAX_ARGS];
int		 	argc = 0;
char*		token;
int 		newCmdReady = 0;

/* Fonctions -----------------------------------------------------------------*/

/**
 * @brief Affiche le message d'acceuil dans le terminal.
 */
void StartShell(){
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
	HAL_Delay(10);
	HAL_UART_Transmit(&huart2, started, sizeof(started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);

	memset(argv,NULL,MAX_ARGS*sizeof(char*));
	memset(cmdBuffer,NULL,CMD_BUFFER_SIZE*sizeof(char));
	memset(uartRxBuffer,NULL,UART_RX_BUFFER_SIZE*sizeof(char));
	memset(uartTxBuffer,NULL,UART_TX_BUFFER_SIZE*sizeof(char));
}

/**
 * @brief Gere l'écriture dans le terminal.
 */
void Shell_Process(){
	if(uartRxReceived){
		switch(uartRxBuffer[0]){
		// Nouvelle ligne, instruction à traiter
		case ASCII_CR:
			HAL_UART_Transmit(&huart2, newLine, sizeof(newLine), HAL_MAX_DELAY);
			cmdBuffer[idx_cmd] = '\0';
			argc = 0;
			token = strtok(cmdBuffer, " ");
			while(token!=NULL){
				argv[argc++] = token;
				token = strtok(NULL, " ");
			}

			idx_cmd = 0;
			newCmdReady = 1;
			break;
			// Suppression du dernier caractère
		case ASCII_DEL:
			cmdBuffer[idx_cmd--] = '\0';
			HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
			break;
			// Nouveau caractère
		default:
			cmdBuffer[idx_cmd++] = uartRxBuffer[0];
			HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		uartRxReceived = 0;
	}
}

/**
 * @brief Gere les entrées de commande dans le terminal.
 */
void New_Command(){
	if(newCmdReady){
		if (strcmp(argv[0], "help") == 0) {
			HAL_UART_Transmit(&huart2, (uint8_t*)help, strlen(help), HAL_MAX_DELAY);
		}
		else if (strcmp(argv[0], "pinout") == 0) {
			HAL_UART_Transmit(&huart2, (uint8_t*)pinout, strlen(pinout), HAL_MAX_DELAY);
		}
		else if (strcmp(argv[0], "start") == 0) {
			StartMotor();
		}
		else if (strcmp(argv[0], "stop") == 0) {
			StopMotor();
		}
		else if (strcmp(argv[0], "speed") == 0) {
			ControlSpeed(argc,argv);
		}
		else if (strcmp(argv[0], "current") == 0) {
			GetCurrent();
		}
		else if (strcmp(argv[0], "vitesse") == 0) {
			getSpeed();
		}

		else{
			HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
		newCmdReady = 0;
	}
}

/**
 * @brief Gere les entrées de commande dans le terminal.
 * @param Pointeur de l'uart recevant le message
 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart){
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
