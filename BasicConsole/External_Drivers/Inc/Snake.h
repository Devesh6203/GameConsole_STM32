/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Snake.h
 * @brief          : Header for Snake.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef _SNAKEGAME_H
#define _SNAKEGAME_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define MAX_SNAKE_LEN		50

#define PIXELSIZE			4		// Pixels taken by single snake bit
#define SNAKESPEED			4

#define SCOREBOARD_HEIGHT	16		// Number of rows given to display score

/*Struct for Snake Game*/
typedef struct{
	uint8_t length;
	uint8_t X[MAX_SNAKE_LEN];
	uint8_t Y[MAX_SNAKE_LEN];
	int8_t 	DirX;
	int8_t 	DirY;
}snakeStructure_t;

typedef struct{
	uint8_t X;
	uint8_t Y;
}foodStructure_t;

/* Function Declarations------------------------------------------------------*/
void snakeGameMain(void);

#ifdef __cplusplus
}
#endif

#endif
