/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Tetris.h
 * @brief          : Header for Tetris.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef _TETRISGAME_H
#define _TETRISGAME_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define GRID_WIDTH		125 		/*SSD1306_WIDTH - 3: To get multiple of 5*/
#define GRID_HEIGHT		45 			/*SSD1306_HEIGHT-16-3: Exclude first 16 rows, as well as adjust to get multiple of 5*/
#define GRID_BLOCK_SIZE	5
#define MARGIN_SIDE		2
#define MARGIN_TOP		17

#define SHAPE_TYPES	6

/*Structure to store game related information*/
typedef struct{
	uint8_t currentShapeType;
	uint8_t nextShapeType;
	uint8_t pieceX;
	uint8_t pieceY;
	uint8_t rotation;
	uint16_t speed;
	char piece[2][4];
}tetrisStructure_t;

/*Enum for shape types---*/
typedef enum{
	L_SHAPE,
	S_SHAPE_LEFT,
	S_SHAPE_RIGHT,
	SQUARE_SHAPE,
	T_SHAPE,
	LINE_SHAPE,
}shapeTypes_t;

/* Function Declarations------------------------------------------------------*/
void tetrisGameMain(void);

#ifdef __cplusplus
}
#endif

#endif
