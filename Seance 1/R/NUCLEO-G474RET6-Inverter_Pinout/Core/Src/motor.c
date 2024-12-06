/**
 * @file motor.c
 * @brief Fichier contenant les fonctions utiles au fonctionnement du moteur.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 5 decembre 2024
 */

/* Includes ------------------------------------------------------------------*/
#include <motor.h>


/* Variables externes --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;

extern const uint8_t powerOn[];
extern const uint8_t powerOff[];
extern const uint8_t wrongargument[];
extern const uint8_t wrongvalue[];

/* Variables globales --------------------------------------------------------*/
int dutycycle;

/* Fonctions -----------------------------------------------------------------*/

/**
 * @brief Modifie le rapport cyclique des PWMs afin de contrôler la vitesse du moteur.
 * @param Rapport cyclique à donner aux PWMs
 * @retval Simple entier pour être sûr de sortir de la fonction.
 */
int control_speed(int dutycycle){
	int current = 100*(TIM1->CCR1)/(TIM1->ARR);
	if(current < dutycycle){
		while(current < dutycycle){
			TIM1->CCR1 += 10;
			TIM1->CCR2 -= 10;
			current = 100*(TIM1->CCR1)/(TIM1->ARR);
			HAL_Delay(100);
		}
		return 1;
	}
	if(current > dutycycle){
		while(current > dutycycle){
			TIM1->CCR1 -= 10;
			TIM1->CCR2 += 10;
			current = 100*(TIM1->CCR1)/(TIM1->ARR);
			HAL_Delay(100);
		}
		return 1;
	}
	return 1;
}

/**
 * @brief Met le rapport cyclique des PWMs à 50% puis les démarre.
 */
void StartMotor(){
	HAL_UART_Transmit(&huart2, (uint8_t*)powerOn, strlen(powerOn), HAL_MAX_DELAY);
	TIM1->CCR1 = 512;
	TIM1->CCR2 = 1023-512;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Met le rapport cyclique des PWMs à 50% puis les arrête.
 */
void StopMotor(){
	HAL_UART_Transmit(&huart2, (uint8_t*)powerOff, strlen(powerOff), HAL_MAX_DELAY);
	control_speed(50);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Teste si le rapport cyclique donné est valide puis le modifie.
 * @param commande entrée dans le terminal
 */
void ControlSpeed(int argc, char ** argv[]){
	if (argc != 2){
		HAL_UART_Transmit(&huart2, (uint8_t*)wrongargument, strlen(wrongargument), HAL_MAX_DELAY);
	}
	else {
		dutycycle = atoi(argv[1]);
		if( (0<dutycycle) && (dutycycle<100) ){
			control_speed(dutycycle);
		}
		else {
			HAL_UART_Transmit(&huart2, (uint8_t*)wrongvalue, strlen(wrongvalue), HAL_MAX_DELAY);
		}
	}
}
