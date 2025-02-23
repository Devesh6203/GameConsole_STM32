/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : OledTask.h
 * @brief          : Header for OledTask.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 */
/* USER CODE END Header */

#ifndef __OLEDTASK_H
#define __OLEDTASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "usart.h"

#include "ssd1306.h"

/*Games Files*/
#include "Snake.h"
#include "Pong.h"
#include "Tetris.h"

/* Defines -------------------------------------------------------------------*/

/*Enum Definition for application state*/
typedef enum{
	APPLICATION_INIT,
	APPLICATION_INIT_COMPLETE,
}applicationStateMachine;

/*Menu Contents enum----*/
typedef enum{
	MAIN_MENU,
	SNAKE_GAME,
	PING_PONG,
	TETRIS_GAME,
	MENU_COUNT,	// Total Contents of the menu
}menuContents_t;

/*Game Info Structure--------*/
typedef enum{
	START,
	EXIT,
	OPTION_COUNT,
}optionContent_t;

typedef enum{
	GAME_IDLE,
	GAME_INPROGRESS,
}gameState_t;

typedef enum{
	GAME_INIT,
	GAME_RUNNING,
}gameStage_t;

typedef struct{
	uint16_t score;			// Score of the current game
	gameState_t state;		// Current state of the game
	gameStage_t stage;		// Current game stage, initilization or running
	uint8_t optionIndex;	// Index to select between start and end
}gameStructure_t;

/*Structure to modify the current game parameters*/
extern gameStructure_t GAME;

/*Variable holding the current application state*/
extern volatile uint8_t applicationState;

/*Flag indicating the status of whether game is running or not*/
extern volatile bool gameRunning;

/*Variable holding the current Menu Index*/
extern volatile uint8_t menuIndex;

/* Task Declaration-----------------------------------------------------------*/
void oLedTask(void* param);

/* Function Declarations------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
