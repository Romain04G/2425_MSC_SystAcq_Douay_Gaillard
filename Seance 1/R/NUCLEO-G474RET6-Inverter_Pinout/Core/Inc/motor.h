/**
 * @file motor.h
 * @brief Fichier contenant les variables utiles pour le fichier motor.c.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 5 decembre 2024
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "shell.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

/* Fonctions -----------------------------------------------------------------*/
int control_speed();
void StartMotor();
void StopMotor();
void ControlSpeed();

#endif /* INC_MOTOR_H_ */
