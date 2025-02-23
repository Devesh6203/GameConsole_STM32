/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : ButtonTask.h
 * @brief          : Header for ButtonTask.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef __BUTTONTASK_H
#define __BUTTONTASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "main.h"
#include "gpio.h"

/* Defines -------------------------------------------------------------------*/
#define LONG_PRESS_THRESHOLD	1000

extern volatile bool shortPressDetected;
/* Task Declaration-----------------------------------------------------------*/
void buttonTask(void* param);

/* Function Declarations------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
