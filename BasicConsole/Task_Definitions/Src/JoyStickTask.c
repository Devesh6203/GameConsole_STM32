/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : joyStickTask.c
 * @brief          : Joystick program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "JoyStickTask.h"

uint32_t XY_AXIS[2];

/*Flag indicating the conversion completion for ADC*/
static bool adcCovCplt = false;

/*Static Function Declarations---------*/
static void updateMenu(void);
static void updateOption(void);
static void readJoyStick(void);

/*Task Definition-----------*/

void joyStickTask(void* param)
{
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	while(applicationState < APPLICATION_INIT_COMPLETE)
	{
		DELAY(1);
	}

	while(1)
	{
		/*Read input from Joystick*/
		readJoyStick();

		if(!gameRunning)		// If in menu state, update the Menu Index
		{
			updateMenu();
		}
		else
		{
			/*
			 * Here, will have two states
			 * State 1: Controlling the game
			 * State 2: After user pressed button, check if he wants to start game again, or to exit
			 */
			switch(GAME.state)
			{
			case GAME_IDLE:
				updateOption();
				break;
			case GAME_INPROGRESS:
				/*
				 * Here, the Joystick buffer is accessed by game
				 */
				break;
			}
		}

		DELAY(50);
	}
}

/*Static Function Definitions--------------*/
/**
 * @brief Handles the menu Index while in menu
 */
static void updateMenu()
{
	/*
	 * Update the menuIndex for OLED task to get account the changes to menu done by joystick
	 *  Menu Index will be updated here based on joystick input
	 * (Note: Needs to take care of menuIndex update only between two update screens in oledTask)
	 */

	if(XY_AXIS[1] > (4095 - JOY_Y_THRES))
	{
		xSemaphoreTake(menuMutex,portMAX_DELAY);
		if(menuIndex < MENU_COUNT - 1)
			menuIndex ++;
		else
			menuIndex = MAIN_MENU + 1;
		xSemaphoreGive(menuMutex);
		DELAY(500);
	}
	else if(XY_AXIS[1] < JOY_Y_THRES)
	{
		xSemaphoreTake(menuMutex,portMAX_DELAY);
		if(menuIndex > MAIN_MENU+1)
			menuIndex --;
		else
			menuIndex = MENU_COUNT-1;
		xSemaphoreGive(menuMutex);
		DELAY(500);
	}
}

/**
 * @brief Handles the option Index when in the game, in IDLE state
 */
static void updateOption()
{
	if(XY_AXIS[1] > (4095 - JOY_Y_THRES))
	{
		xSemaphoreTake(menuMutex,portMAX_DELAY);
		if(GAME.optionIndex < OPTION_COUNT - 1)
			GAME.optionIndex++;
		else
			GAME.optionIndex = 0;
		xSemaphoreGive(menuMutex);
		DELAY(500);
	}
	else if(XY_AXIS[1] < JOY_Y_THRES)
	{
		xSemaphoreTake(menuMutex,portMAX_DELAY);
		if(GAME.optionIndex > 0)
			GAME.optionIndex --;
		else
			GAME.optionIndex = OPTION_COUNT - 1;
		xSemaphoreGive(menuMutex);
		DELAY(500);
	}
}

/**
 * @brief Reads the Joystick value and gets the x-axis and y-axis value
 */
static void readJoyStick()
{
	HAL_ADC_Start_DMA(&hadc1, XY_AXIS, 2);
	while(!adcCovCplt)
		DELAY(1);

	adcCovCplt = false;
}

/*Callback Handling------------*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1)
		adcCovCplt = true;
}
