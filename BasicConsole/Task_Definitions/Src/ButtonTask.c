/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : ButtonTask.c
 * @brief          : Button program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "ButtonTask.h"

volatile bool shortPressDetected = false;

static uint32_t pressTime;

/*Static Function Declarations---------*/
static void handleLongPress(void);

/*Task Definition-----------*/

void buttonTask(void* param)
{
	while(applicationState < APPLICATION_INIT_COMPLETE)
	{
		DELAY(1);
	}

	while(1)
	{
		if(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
		{
//			DELAY(1000);		// Button Debouncing Handle
//			if(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)		// Long Press Detected
//			{
//				handleLongPress();
//			}
			DELAY(50);	// Debounce Delay

			if(HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
			{
				pressTime = 0;
				while (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)
				{
					DELAY(1);  // 1ms delay
					pressTime++;

					// Handle long press immediately when threshold is reached
					if (pressTime >= LONG_PRESS_THRESHOLD)
					{
						handleLongPress();
						while (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET)	// Wait for release
						{
							DELAY(1);
						}
					}
				}

				// If button is released before reaching the long press threshold, it's a short press
				if(pressTime < LONG_PRESS_THRESHOLD)
					shortPressDetected = true;
			}
		}
	}
}

/*Static Function Definitions--------------*/
static void handleLongPress()
{
	/*
	 * Handling of button press will be done
	 * If in menu, handles going to the current menuIndex pointed
	 * If in game, handles exiting the game
	 */

	if(!gameRunning)		// Selecting the game from menu
	{
		gameRunning = true;
		/*
		 * Here, will set the scores of game selected to zero, and state of game
		 */
		GAME.score = 0;
		GAME.state = GAME_IDLE;
		GAME.optionIndex = START;

		DELAY(1000);		// Avoid handling of button press for 1 second (To handle immediate exit condition)
	}
	else					// Exiting the game
	{
		/*
		 * gameRunning should be set to false by oledtask
		 * The game should first display the final score of current game.
		 * Ask user to exit or to play again
		 */
		if(GAME.state == GAME_IDLE)
		{
			xSemaphoreTake(menuMutex,portMAX_DELAY);
			if(GAME.optionIndex == START)	// If start is pressed, enter the game
			{
				// Initializes game before starting
				GAME.state = GAME_INPROGRESS;
				GAME.score = 0;
				GAME.stage = GAME_INIT;
			}
			else
				gameRunning = false;		// Exiting the game, to display the main menu
			xSemaphoreGive(menuMutex);
		}
		else
		{
			/*Change state to game idle, where OLED task will display score*/
			GAME.state = GAME_IDLE;
		}
	}
}
