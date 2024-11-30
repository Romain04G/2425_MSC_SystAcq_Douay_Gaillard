/**
 * @file functions.c
 * @brief File containing all the functions used.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 30 novembre 2024
 */

/* Includes ------------------------------------------------------------------*/
#include "function.h"

/* Variables externes --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Fonctions -----------------------------------------------------------------*/

/**
 * @brief Modify the dutycycle of the PWM to control the motor speed.
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
 * @brief Convert the ADC value into the current value.
 */
float ConvertADCToCurrent(uint32_t adc_value) {
	// Calcul de la tension en volts
	float voltage = (adc_value / ADC_RESOLUTION) * VREF;

	// Conversion en courant selon la fonction de transfert
	return (voltage - V_OFFSET) / SENSITIVITY;
}

/**
 * @brief Start the PWM with a dutycycle of 50%.
 */
void StartMotor(void){
	HAL_UART_Transmit(&huart2, (uint8_t*)powerOn, strlen(powerOn), HAL_MAX_DELAY);
	TIM1->CCR1 = 512;
	TIM1->CCR2 = 1023-512;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Set the dutycycle to 50% (motor stop) then stop the PWM.
 */
void StopMotor(void){
	HAL_UART_Transmit(&huart2, (uint8_t*)powerOff, strlen(powerOff), HAL_MAX_DELAY);
	control_speed(50);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Modify the speed of the motor.
 */
void SetSpeed(void){
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

/**
 * @brief Get the current value.
 */
void GetCurrent(void){
	float voltage = ConvertADCToCurrent(adc_buffer[0]);  // Convertir en tension
	sprintf(buffer, "Voltage (DMA): %.2f V\r\n", voltage);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
