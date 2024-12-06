/**
 * @file shell.h
 * @brief Fichier contenant les variables utiles pour le fichier shell.c.
 * @author Nicolas DOUAY / Romain GAILLARD
 * @date 5 decembre 2024
 */

#ifndef INC_SHELL_H_
#define INC_SHELL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <measure2.h>
#include <motor.h>
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"

/* Defines -------------------------------------------------------------------*/
#define CMD_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64
#define MAX_ARGS 9

// LF = line feed, saut de ligne
#define ASCII_LF 0x0A
// CR = carriage return, retour chariot
#define ASCII_CR 0x0D
// DEL = delete
#define ASCII_DEL 0x7F

/* Fonctions -----------------------------------------------------------------*/
void StartShell();
void Shell_Process();
void New_Command();

#endif /* INC_SHELL_H_ */
