/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Snake.c
 * @brief          : Snake Game program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "Snake.h"

static snakeStructure_t SNAKE;
static foodStructure_t FOOD;

/*Buffer to print score*/
static char scoreBuff[20];

static bool gameOver = false;

/*Static function declarations*/;
static void generateFood(void);
static void resetGame(void);

static void readJoyStick(void);

static void updateSnakePosition(void);
static void drawBorders(void);
static void drawFood(void);
static void drawSnake(void);
static void displayScore(void);
static void displayGameOver(void);

/*Function Definitions*/
/**
 * @brief Main Function to handle the snake game
 */
void snakeGameMain(void)
{
	switch(GAME.stage)
	{
	case GAME_INIT:
		/*Reset the paramerts of the game to default*/
		resetGame();
		SSD1306_Fill(SSD1306_COLOR_BLACK);
		GAME.stage = GAME_RUNNING;
		gameOver = false;
		break;
	case GAME_RUNNING:
		readJoyStick();

		updateSnakePosition();

		if(!gameOver)		// If Game is over, then "GAME Over" message will be displayed
		{
			drawBorders();
			displayScore();
			drawFood();
			drawSnake();
			SSD1306_UpdateScreen();
			DELAY(50);		// Adding extra delay to slow the snake's speed
		}

		break;
	}
}

/*Static Function Definitions*/
/**
 * @brief Generated food on OLED for snake
 */
static void generateFood()
{
	/*
	 * Generated food point on available game display, ensuring it don't appear on edge
	 */
	FOOD.X = ((rand() % ((SSD1306_WIDTH / PIXELSIZE) - 2)) + 1) * PIXELSIZE;
	FOOD.Y = (((rand() % ((SSD1306_HEIGHT - SCOREBOARD_HEIGHT) / PIXELSIZE - 2)) + 1) * PIXELSIZE) + SCOREBOARD_HEIGHT;;
}

/**
 * @brief Resets the parameters for snake game
 */
static void resetGame()
{
	SNAKE.length = 1;
	SNAKE.X[0] = (SSD1306_WIDTH/2/PIXELSIZE)*PIXELSIZE;
	SNAKE.Y[0] = (SSD1306_HEIGHT/2/PIXELSIZE)*PIXELSIZE;
	SNAKE.DirX = SNAKESPEED;
	SNAKE.DirY = 0;

	generateFood();
}

/**
 *@brief Reads the joystick and updates the snake direction of moving accordingly
 */
static void readJoyStick(void)
{
	if(XY_AXIS[0] > (4095 - JOY_X_THRES) && SNAKE.DirX == 0)
	{
		SNAKE.DirY = 0;
		SNAKE.DirX = SNAKESPEED;
	}
	else if(XY_AXIS[0] < JOY_X_THRES && SNAKE.DirX == 0)
	{
		SNAKE.DirY = 0;
		SNAKE.DirX = -SNAKESPEED;
	}
	else if(XY_AXIS[1] > (4095 - JOY_Y_THRES) && SNAKE.DirY == 0)
	{
		SNAKE.DirX = 0;
		SNAKE.DirY = SNAKESPEED;
	}
	else if(XY_AXIS[1] < JOY_Y_THRES && SNAKE.DirY == 0)
	{
		SNAKE.DirX = 0;
		SNAKE.DirY = -SNAKESPEED;
	}
}

/**
 * @brief Updates the position of the snake
 */
static void updateSnakePosition(void)
{
	/*Moves the Snake's body*/
	for(int i=SNAKE.length - 1; i>0; i--)
	{
		SNAKE.X[i] = SNAKE.X[i-1];
		SNAKE.Y[i] = SNAKE.Y[i-1];
	}

	/*Moves the Snake's head*/
	SNAKE.X[0] += SNAKE.DirX;
	SNAKE.Y[0] += SNAKE.DirY;

	/*Eating food point*/
	if(SNAKE.X[0] == FOOD.X && SNAKE.Y[0] == FOOD.Y)
	{
		if(SNAKE.length < MAX_SNAKE_LEN)
			SNAKE.length++;

		GAME.score++;

		generateFood();
	}

	/*Handling collision with border*/
	if(SNAKE.X[0] < 0 || SNAKE.X[0] >= SSD1306_WIDTH || SNAKE.Y[0] < SCOREBOARD_HEIGHT || SNAKE.Y[0] >= SSD1306_HEIGHT)
	{
		displayGameOver();
		DELAY(2000);
		GAME.state = GAME_IDLE;
		return;
	}

	/*Handling collision with itself*/
	for(int i=1; i< SNAKE.length; i++)
	{
		if(SNAKE.X[0] == SNAKE.X[i] && SNAKE.Y[0] == SNAKE.Y[i])
		{
			displayGameOver();
			DELAY(2000);
			GAME.state = GAME_IDLE;
			return;
		}
	}
}

/**
 * @brief Displays the border on the OLED
 */
static void drawBorders(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	for (int i = 0; i < SSD1306_WIDTH; i++)
	{
		SSD1306_DrawPixel(i, SCOREBOARD_HEIGHT, 1);
		SSD1306_DrawPixel(i, SSD1306_HEIGHT - 1, 1);
	}
	for (int i = SCOREBOARD_HEIGHT; i < SSD1306_HEIGHT; i++) {
		SSD1306_DrawPixel(0, i, 1);
		SSD1306_DrawPixel(SSD1306_WIDTH - 1, i, 1);
	}

}

/**
 * @brief Displays score on the top of OLED screen
 */
static void displayScore(void)
{
	SSD1306_GotoXY(0,0);
	sprintf(scoreBuff,"Score: %d",GAME.score);
	SSD1306_Puts(scoreBuff,&Font_7x10,1);
}

/**
 * @brief Displays the food on the OLED
 */
static void drawFood(void)
{
	for(int i=0;i<PIXELSIZE;i++)
	{
		for(int j=0;j<PIXELSIZE;j++)
			SSD1306_DrawPixel(FOOD.X+i, FOOD.Y+j, 1);
	}
}

/**
 * @brief Displays the snake on the OLED
 */
static void drawSnake(void)
{
	for(int i=0; i< SNAKE.length;i++)
	{
		for(int x=0;x<PIXELSIZE;x++)
		{
			for(int y=0;y<PIXELSIZE;y++)
				SSD1306_DrawPixel(SNAKE.X[i] + x, SNAKE.Y[i] + y, 1);
		}
	}
}

/**
 * @brief Displays the Game Over Message
 */
static void displayGameOver(void)
{
	gameOver = true;

	SSD1306_GotoXY (30,18);
	SSD1306_Puts ("GAME", &Font_11x18, 1);
	SSD1306_GotoXY (30, 40);
	SSD1306_Puts ("OVER", &Font_11x18, 1);
	SSD1306_UpdateScreen(); // update screen
}
