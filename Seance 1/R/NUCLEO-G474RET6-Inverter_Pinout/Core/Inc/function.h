/*
 * functions.h
 *
 *  Created on: Nov 30, 2024
 *      Author: Nicolas DOUAY / Romain GAILLARD
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

int control_speed();
float ConvertADCToCurrent();
void StartMotor();
void StopMotor();
void SetSpeed();
void GetCurrent();
