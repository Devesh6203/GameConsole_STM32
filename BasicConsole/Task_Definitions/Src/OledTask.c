/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : OledTask.c
 * @brief          : OLED program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "OledTask.h"

volatile bool gameRunning = false;
volatile uint8_t applicationState = APPLICATION_INIT;
volatile uint8_t menuIndex = MAIN_MENU+1;		// Current menu index

static char buff[30];		// Buffer used to display message

/*Menu Options for the main menu*/
static char* menuItems[MENU_COUNT] = {"Menu","Snake","Pong","Tetris"};

/*Options to start or exit the game in IDLE state*/
static char* options[OPTION_COUNT] = {"Start","Exit"};

/*Structure to modify the current game parameters*/
gameStructure_t GAME;

/*Static Function Declarations---------*/
static void displayMenu(void);

static void snakeGame(void);
static void pongGame(void);
static void tetrisGame(void);

/*Task Definition-----------*/

void oLedTask(void* param)
{
	/*Initialize the OLED Here*/
	SSD1306_Init (); // initialise the display

	/*Displaying start up message*/
	SSD1306_GotoXY (40,16); // goto 10, 10
	SSD1306_Puts ("GAME", &Font_11x18, 1); // print Hello
	SSD1306_GotoXY (30, 40);
	SSD1306_Puts ("CONSOLE", &Font_11x18, 1);
	SSD1306_UpdateScreen(); // update screen

	DELAY(2000);

	applicationState = APPLICATION_INIT_COMPLETE;

	while(1)
	{
		if(!gameRunning)		// Have OLED Display control till game is selected
		{
			displayMenu();
		}
		else
		{
			switch(menuIndex)		// menuIndex here holds the latest Index selected by
			{
			case SNAKE_GAME:
				snakeGame();
				break;
			case PING_PONG:
				pongGame();
				break;
			case TETRIS_GAME:
				tetrisGame();
				break;
			}
		}

		DELAY(10);
	}
}

/*Static Function Definitions--------------*/
static void displayMenu()
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	/*Center Aligning the MENU*/
	SSD1306_GotoXY((SSD1306_WIDTH - (strlen(menuItems[0])*11))/2, 0);
	SSD1306_Puts(menuItems[0],&Font_11x18,1);

	/*Display the contents of the menu*/
	xSemaphoreTake(menuMutex,portMAX_DELAY);
	for(int i=1; i< MENU_COUNT; i++)
	{
		SSD1306_GotoXY(0,16 + i*10);
		if(i == menuIndex)
		{
			SSD1306_Puts(">",&Font_7x10,1);
		}
		else
		{
			SSD1306_Puts(" ",&Font_7x10,1);
		}
		SSD1306_Puts(menuItems[i],&Font_7x10,1);
	}
	xSemaphoreGive(menuMutex);

	SSD1306_UpdateScreen();
}

static void snakeGame(void)
{
	switch(GAME.state)
	{
	case GAME_IDLE:
		SSD1306_Fill(SSD1306_COLOR_BLACK);

		SSD1306_GotoXY((SSD1306_WIDTH - (strlen("SNAKE")*11))/2, 0);
		SSD1306_Puts("SNAKE",&Font_11x18,1);

		SSD1306_GotoXY(0,16);						// Start of main Screen Display
		sprintf(buff,"Game Score: %d",GAME.score);
		SSD1306_Puts(buff,&Font_7x10,1);	// Displaying game score

		xSemaphoreTake(menuMutex,portMAX_DELAY);		// Make sure, optionIndex is not updated while forming buffer
		for(int i=OPTION_COUNT - 1; i>=0; i--)
		{
			SSD1306_GotoXY(0,63 - 10*(OPTION_COUNT - i));
			if(i == GAME.optionIndex)
			{
				SSD1306_Puts(">",&Font_7x10,1);
			}
			else
			{
				SSD1306_Puts(" ",&Font_7x10,1);
			}
			SSD1306_Puts(options[i],&Font_7x10,1);
		}
		xSemaphoreGive(menuMutex);

		SSD1306_UpdateScreen();
		break;
	case GAME_INPROGRESS:
		snakeGameMain();
		break;
	}
}

static void pongGame(void)
{
	switch(GAME.state)
	{
	case GAME_IDLE:
		SSD1306_Fill(SSD1306_COLOR_BLACK);

		SSD1306_GotoXY((SSD1306_WIDTH - (strlen("PING PONG")*11))/2, 0);
		SSD1306_Puts("PING PONG",&Font_11x18,1);

		SSD1306_GotoXY(0,16);						// Start of main Screen Display
		sprintf(buff,"Game Score: %d",GAME.score);
		SSD1306_Puts(buff,&Font_7x10,1);	// Displaying game score

		xSemaphoreTake(menuMutex,portMAX_DELAY);		// Make sure, optionIndex is not updated while forming buffer
		for(int i=OPTION_COUNT - 1; i>=0; i--)
		{
			SSD1306_GotoXY(0,63 - 10*(OPTION_COUNT - i));
			if(i == GAME.optionIndex)
			{
				SSD1306_Puts(">",&Font_7x10,1);
			}
			else
			{
				SSD1306_Puts(" ",&Font_7x10,1);
			}
			SSD1306_Puts(options[i],&Font_7x10,1);
		}
		xSemaphoreGive(menuMutex);

		SSD1306_UpdateScreen();
		break;

	case GAME_INPROGRESS:
		pongGameMain();
		break;
	}
}

static void tetrisGame(void)
{
	switch(GAME.state)
	{
	case GAME_IDLE:
		SSD1306_Fill(SSD1306_COLOR_BLACK);

		SSD1306_GotoXY((SSD1306_WIDTH - (strlen("TETRIS")*11))/2, 0);
		SSD1306_Puts("TETRIS",&Font_11x18,1);

		SSD1306_GotoXY(0,16);						// Start of main Screen Display
		sprintf(buff,"Game Score: %d",GAME.score);
		SSD1306_Puts(buff,&Font_7x10,1);	// Displaying game score

		xSemaphoreTake(menuMutex,portMAX_DELAY);		// Make sure, optionIndex is not updated while forming buffer
		for(int i=OPTION_COUNT - 1; i>=0; i--)
		{
			SSD1306_GotoXY(0,63 - 10*(OPTION_COUNT - i));
			if(i == GAME.optionIndex)
			{
				SSD1306_Puts(">",&Font_7x10,1);
			}
			else
			{
				SSD1306_Puts(" ",&Font_7x10,1);
			}
			SSD1306_Puts(options[i],&Font_7x10,1);
		}
		xSemaphoreGive(menuMutex);

		SSD1306_UpdateScreen();
		break;
	case GAME_INPROGRESS:
		tetrisGameMain();
		break;
	}
}
