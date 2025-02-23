/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : joyStickTask.h
 * @brief          : Header for joyStickTask.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef __JOYSTICKTASK_H
#define __JOYSTICKTASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include  "main.h"
#include "adc.h"
#include "usart.h"

/* Defines -------------------------------------------------------------------*/

#define JOY_X_THRES		300

#define JOY_Y_THRES		300

extern uint32_t XY_AXIS[2];

/* Task Declaration-----------------------------------------------------------*/
void joyStickTask(void* param);

/* Function Declarations------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
