/**
 * @file measure.c
 * @brief Fichier contenant les fonctions utiles pour la mesure du courant et de la vitesse de rotation.
 * @author Nicolas SIMOND
 * @date 8 novembre 2024
 */

/* Includes ------------------------------------------------------------------*/
#include <measure2.h>

/* Variables externes --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

/* Variables globales --------------------------------------------------------*/
float 		adc_voltage;
float 		speed;
float 		voltage;
float 		curent;
uint16_t 	adc_buffer2[ADC_BUFFER_SIZE];  		// Tampon pour les données ADC
uint16_t 	adc_buffer[ADC_BUFFER_SIZE];  		// Tampon pour les données ADC

/* Fonctions -----------------------------------------------------------------*/

/**
 * @brief Démarre l'ADC.
 */
void StartADC(){
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	if (HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUFFER_SIZE) == HAL_OK) {
		char buff[50];
		sprintf(buff, "ADC DMA started successfully\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
	} else {
		char buff[50];
		sprintf(buff, "ADC DMA start failed\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
	}

	if (HAL_ADC_Start_DMA(&hadc2, adc_buffer2, ADC_BUFFER_SIZE2) == HAL_OK) {
		char buff[50];
		sprintf(buff, "ADC DMA started successfully\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
	} else {
		char buff[50];
		sprintf(buff, "ADC DMA start failed\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
	}
}

/**
 * @brief Mesure le courant et la vitesse du moteur.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) //courant
	{
		// Conversion terminée
		voltage = (adc_buffer[0] / ADC_RESOLUTION) * VREF;

		// Calculer le courant
		curent = (voltage - V_OFFSET) / SENSITIVITY;
	}
	if (hadc->Instance == ADC2) //vitesse
	{
		// Lecture de la tension via l'ADC
		adc_voltage = adc_buffer2[1] * (VREF / ADC_RESOLUTION); // Conversion ADC -> Tension
		float sensor_input = adc_voltage / HCPL_GAIN;  // Recalcule l'entrée du capteur
		speed = sensor_input / TACH_GAIN;             // Calcul de la vitesse
	}
}

/**
 * @brief Affiche le courant dans le moteur dans le terminal.
 */
void GetCurrent(){
	char buffer[50];
	sprintf(buffer, "ADC Value: %lu, Voltage: %.2f V, Current: %.2f A\r\n", adc_buffer[0], voltage, curent);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

/**
 * @brief Affiche la vitesse de rotation du moteur dans le terminal.
 */
void getSpeed(){
	char buffer[50];
	sprintf(buffer, "ADC Value: %lu, Voltage: %.2f V, Vitesse: %.2f rad/s\r\n", adc_buffer2[1], adc_voltage, speed);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}




