/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Pong.c
 * @brief          : Ping-Pong Game program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "Pong.h"

static pongStructure_t PONG;
static bool gameOver = false;

/*Buffer to print score*/
static char scoreBuff[20];

/*Static Function Declarations*/
static void resetGame(void);

static void readJoyStick(void);

static void updatePaddlePos(void);
static void updateBallPos(void);
static void drawBorders(void);
static bool ballatBottom(void);
static bool ballHittingPaddle(void);
static void displayScore(void);
static void displayFrame(void);
static void displayGameOver(void);

/*Function Definitions*/
/**
 * @brief Main Function to handle the pong game
 */
void pongGameMain(void)
{
	switch(GAME.stage)
	{
	case GAME_INIT:
		/*Reset the paramerts of the game to default*/
		resetGame();
		GAME.stage = GAME_RUNNING;
		gameOver = false;
		break;
	case GAME_RUNNING:
		readJoyStick();
		updatePaddlePos();

		if(ballatBottom())		// New ball position has reached the bottom
		{
			if(!ballHittingPaddle())
			{
				gameOver = true;
				displayGameOver();
				DELAY(2000);
				GAME.state = GAME_IDLE;
			}
			else
			{
				GAME.score++;
				/*Debounce the ball off the paddle*/
				updateBallPos();

				if(GAME.score%5 == 0)	// At each 5 points, speed of ball is increased
				{
					PONG.ballVel_X += (PONG.ballVel_X<0) ? -1 : 1;
					PONG.ballVel_Y += (PONG.ballVel_Y<0) ? -1 : 1;
				}
			}
		}
		else
			updateBallPos();

		if(!gameOver)
			displayFrame();

		break;
	}
}

/*Static Function Definitions*/
/**
 * @brief Resets the parameters for pong game
 */
static void resetGame()
{
	PONG.paddlePos = 30;
	PONG.ballVel_X = -3;
	PONG.ballVel_Y = 2;
	PONG.ballX = 100;
	PONG.ballY = 30;
}

/**
 *@brief Reads the joystick
 */
static void readJoyStick(void)
{
	if(XY_AXIS[1] > (4095 - JOY_Y_THRES))
	{
		PONG.paddleDir = PADDLE_RIGHT;		// Will move to the right
	}
	else if(XY_AXIS[1] < JOY_Y_THRES)
	{
		PONG.paddleDir = PADDLE_LEFT;		// Will move to the left
	}
	else
	{
		PONG.paddleDir = PADDLE_NONE;		// Will not move the paddle
	}
}

/**
 *@brief  Updates the position of the paddle
 */
static void updatePaddlePos(void)
{
	switch(PONG.paddleDir)
	{
	case PADDLE_LEFT:
		PONG.paddlePos = PONG.paddlePos - PADDLE_SPEED;
		break;
	case PADDLE_RIGHT:
		PONG.paddlePos = PONG.paddlePos + PADDLE_SPEED;
		break;
	case PADDLE_NONE:
		break;
	}

	if(PONG.paddlePos <= SCOREBOARD_HEIGHT+1)		// Excluding the first 16 rows for game display
		PONG.paddlePos = SCOREBOARD_HEIGHT+1;
	else if(PONG.paddlePos > SSD1306_HEIGHT - PADDLE_WIDTH-1)
	{
		PONG.paddlePos = SSD1306_HEIGHT -1 - PADDLE_WIDTH;
	}
}

/**
 * @brief Displays the border on the OLED
 */
static void drawBorders(void)
{
	for (int i = 0; i < SSD1306_WIDTH; i++)
	{
		SSD1306_DrawPixel(i, SCOREBOARD_HEIGHT, 1);
		SSD1306_DrawPixel(i, SSD1306_HEIGHT - 1, 1);
	}
}

/**
 * @brief Updates position of ball
 * Note: Works on reflection formula :	x' = B - (x - B), where
 * 										B = Boundary
 * 										x = previous position
 * 										x' = mirrored position
 */
static void updateBallPos()
{
	PONG.ballX += PONG.ballVel_X;
	PONG.ballY += PONG.ballVel_Y;

	/*Handling the collisions of ball with side surfaces*/
	if(PONG.ballY <= (SCOREBOARD_HEIGHT + BALL_RADIUS) && PONG.ballVel_Y < 0)
	{
		PONG.ballY = 2*(SCOREBOARD_HEIGHT+BALL_RADIUS) - PONG.ballY;
		PONG.ballVel_Y = -PONG.ballVel_Y;
	}
	else if(PONG.ballY > (SSD1306_HEIGHT - BALL_RADIUS - 2) && PONG.ballVel_Y > 0)
	{
		PONG.ballY =  2*(SSD1306_HEIGHT - BALL_RADIUS) - PONG.ballY;
		PONG.ballVel_Y = -PONG.ballVel_Y;
	}

	/*Handling the collision of ball from top surface*/
	if(PONG.ballX > (SSD1306_WIDTH - BALL_RADIUS - 1) && PONG.ballVel_X > 0)
	{
		PONG.ballX =  2*(SSD1306_WIDTH - BALL_RADIUS) - PONG.ballX;
		PONG.ballVel_X = -PONG.ballVel_X;
	}
	else if(PONG.ballX < (PADDLE_HEIGHT + BALL_RADIUS-1) && PONG.ballVel_X < 0)
	{
		PONG.ballX =  2*(PADDLE_HEIGHT + BALL_RADIUS-1) - PONG.ballX;
		PONG.ballVel_X = -PONG.ballVel_X;
	}
}

/**
 * @brief Checks whether ball will reach the paddle height in next frame
 */
static bool ballatBottom()
{
	uint8_t nextFrame_X = PONG.ballX + PONG.ballVel_X;

	if(nextFrame_X < (PADDLE_HEIGHT + BALL_RADIUS-1) && PONG.ballVel_X < 0)
		return true;
	else
		return false;
}

/**
 * @brief Checks whether ball is hitting paddle
 */
static bool ballHittingPaddle(void)
{
	uint8_t nextFrame_Y = PONG.ballY + PONG.ballVel_Y;

	if((nextFrame_Y >= PONG.paddlePos) && nextFrame_Y <= (PONG.paddlePos+PADDLE_WIDTH))
		return true;
	else
		return false;
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
 * @brief Displays the current frame of the game
 */
static void displayFrame(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	displayScore();
	drawBorders();
	SSD1306_DrawFilledRectangle(0,PONG.paddlePos, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_COLOR_WHITE);
	SSD1306_DrawFilledCircle(PONG.ballX, PONG.ballY, BALL_RADIUS, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();

	// Provides a variable delay as score increases
	DELAY(MAX(30 - (int16_t)(2 * GAME.score),1));
}

/**
 * @brief Displays the Game Over Message
 */
static void displayGameOver(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	displayScore();
	drawBorders();
	updateBallPos();
	SSD1306_DrawFilledRectangle(0,PONG.paddlePos, PADDLE_HEIGHT, PADDLE_WIDTH, SSD1306_COLOR_WHITE);
	SSD1306_DrawFilledCircle(PONG.ballX, PONG.ballY, BALL_RADIUS, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY (30,18);
	SSD1306_Puts ("GAME", &Font_11x18, 1);
	SSD1306_GotoXY (30, 40);
	SSD1306_Puts ("OVER", &Font_11x18, 1);
	SSD1306_UpdateScreen(); // update screen
}
