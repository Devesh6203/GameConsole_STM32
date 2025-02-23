/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Pong.h
 * @brief          : Header for Pong.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef _PONGGAME_H
#define _PONGGAME_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define PADDLE_WIDTH	12
#define PADDLE_HEIGHT	2

#define BALL_RADIUS		3

#define PADDLE_SPEED	3

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*Struct for Pong Game*/
typedef enum{
	PADDLE_LEFT,
	PADDLE_RIGHT,
	PADDLE_NONE,
}paddleMovDir_t;

typedef struct{
	uint8_t paddlePos;
	uint8_t ballX;
	uint8_t ballY;
	int8_t ballVel_X;
	int8_t ballVel_Y;
	paddleMovDir_t paddleDir;
}pongStructure_t;

/* Function Declarations------------------------------------------------------*/
void pongGameMain(void);

#ifdef __cplusplus
}
#endif

#endif
