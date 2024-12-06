/**
 * @file measure.h
 * @brief Fichier contenant les variables utiles pour le fichier measure.c.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 5 decembre 2024
 */

#ifndef INC_MEASURE_H_
#define INC_MEASURE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

/* Defines -------------------------------------------------------------------*/
#define 	VREF 3.3f                			// Tension de référence de l'ADC
#define 	ADC_RESOLUTION 4096.0f   			// Résolution de l'ADC
#define 	V_OFFSET 1.65f           			// Tension de repos à courant nul
#define 	SENSITIVITY 0.05f         			// Sensibilité du capteur (en V/A)

#define 	HCPL_GAIN 4.0f; 					// Gain typique du HCPL-7800
#define 	TACH_GAIN 0.02f; 					// Gain de la sonde tachymétrique (en V/(rad/s))

#define 	ADC_BUFFER_SIZE 1  					// Nombre de canaux à convertir
#define 	ADC_BUFFER_SIZE2 2  				// Nombre de canaux à convertir

/* Fonctions -----------------------------------------------------------------*/
void StartADC();
void HAL_ADC_ConvCpltCallback();
void GetCurrent();
void getSpeed();

#endif /* INC_MEASURE_H_ */
