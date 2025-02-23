/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Tetris.c
 * @brief          : Tetris Game program body
 ******************************************************************************
 */
/* USER CODE END Header */

#include "Tetris.h"

/*
 * The below arrays hold the shapes for tetris games
 * First index : the number of rotations of the piece,
 * Second index: the number of rows, and
 * Third index : the number of columns.
 */
/*"S" shape rotated left*/
const char pieces_S_l[2][2][4] =
{
		{
				{0, 0, 1, 1},
				{0, 1, 1, 2}
		},
		{
				{0, 1, 1, 2},
				{1, 1, 0, 0}
		}
};

/*"S" shape rotated right*/
const char pieces_S_r[2][2][4] =
{
		{
				{1, 1, 0, 0},
				{0, 1, 1, 2}
		},
		{
				{0, 1, 1, 2},
				{0, 0, 1, 1}
		}
};

/*"L" shape rotated left*/
const char pieces_L_l[4][2][4] =
{
		{
				{0, 0, 0, 1},
				{0, 1, 2, 2}
		},
		{
				{0, 1, 2, 2},
				{1, 1, 1, 0}
		},
		{
				{0, 1, 1, 1},
				{0, 0, 1, 2}
		},
		{
				{0, 0, 1, 2},
				{1, 0, 0, 0}
		}
};

/*"Square" shape*/
const char pieces_Sq[1][2][4] =
{
		{
				{0, 1, 0, 1},
				{0, 0, 1, 1}
		}
};

/*"T" shape*/
const char pieces_T[4][2][4] =
{
		{
				{0, 0, 1, 0},
				{0, 1, 1, 2}
		},
		{
				{0, 1, 1, 2},
				{1, 0, 1, 1}
		},
		{
				{1, 0, 1, 1},
				{0, 1, 1, 2}
		},
		{
				{0, 1, 1, 2},
				{0, 0, 1, 0}
		}
};

/*"Line" shape*/
const char pieces_l[2][2][4] =
{
		{
				{0, 1, 2, 3},
				{0, 0, 0, 0}
		},
		{
				{0, 0, 0, 0},
				{0, 1, 2, 3}
		}
};

static tetrisStructure_t TETRIS;
static bool grid[GRID_WIDTH/GRID_BLOCK_SIZE][GRID_HEIGHT/GRID_BLOCK_SIZE];

/*Buffer to print score*/
static char scoreBuff[20];

static bool gameOver = false;

/*Static function declarations*/;
static void resetGame(void);

static void readJoyStick(void);

static void drawBorders(void);
static void displayScore(void);

static void copyPiece(char piece[2][4], uint8_t type, uint8_t rotation);
static void drawNextPiece(void);
static void generatePiece(void);

static uint8_t getMaxRotation(uint8_t type);
static bool canRotate(uint8_t rotation);

static bool ifCollision(void);

static void checkLines(void);
static void breakLine(uint8_t line);

static void displayFrame(void);
static void displayGameOver(void);
/*Function Definitions*/
/**
 * @brief Main Function to handle the Tetris game
 */
static void refresh(void);

void tetrisGameMain(void)
{
	switch(GAME.stage)
	{
	case GAME_INIT:
		/*Reset the parameters of the game to default*/
		resetGame();
		GAME.stage = GAME_RUNNING;
		gameOver = false;
		break;
	case GAME_RUNNING:
		refresh();
		checkLines();
		if(ifCollision())
		{
			for(int i=0;i<4;i++)
				grid[TETRIS.pieceX + TETRIS.piece[0][i]][TETRIS.pieceY + TETRIS.piece[1][i]] = true;

			for(int i=0;i<GRID_HEIGHT/GRID_BLOCK_SIZE;i++)
			{
				/*Checking if grid topmost cell is full, then Game over*/
				if(grid[GRID_WIDTH/GRID_BLOCK_SIZE - 5][i])
				{
					gameOver = true;
					displayGameOver();
					DELAY(2000);
					GAME.state = GAME_IDLE;
					break;
				}
			}

			generatePiece();
		}
		else
			TETRIS.pieceX --;

		readJoyStick();

		DELAY(TETRIS.speed);
		break;
	}
}

/*Static Function Definitions*/
/**
 * @brief Resets the parameters for tetris game
 */
static void resetGame()
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	displayFrame();

	/*Initilaizing speed for pieces movement*/
	TETRIS.speed = 250;
	TETRIS.nextShapeType = rand()%SHAPE_TYPES;
	memset(grid,0,sizeof(grid));

	/*Discarding any short press before starting game*/
	shortPressDetected = false;

	/*Generates random piece for start of game*/
	generatePiece();
}

/**
 *@brief Reads the joystick input
 */
static void readJoyStick(void)
{
	int newY;
	bool canMove = true;

	if(XY_AXIS[1] > (4095 - JOY_Y_THRES))
	{
		for(int i=0;i<4;i++)	 // Loop through all 4 blocks of the piece
		{
			newY = TETRIS.pieceY + TETRIS.piece[1][i] + 1;  // Calculate new Y position of the block

			// Check if the new position is outside the game board or collides with an existing block
			if(newY >= GRID_HEIGHT/GRID_BLOCK_SIZE || grid[TETRIS.pieceX + TETRIS.piece[0][i]][newY])
			{
				canMove = false;
				break;
			}
		}
		if(canMove)
			TETRIS.pieceY++;
	}
	else if(XY_AXIS[1] < JOY_Y_THRES)
	{
		for(int i=0;i<4;i++)	 // Loop through all 4 blocks of the piece
		{
			newY = TETRIS.pieceY + TETRIS.piece[1][i] - 1;  // Calculate new Y position of the block

			// Check if the new position is outside the game board or collides with an existing block
			if(newY < 0 || grid[TETRIS.pieceX + TETRIS.piece[0][i]][newY])
			{
				canMove = false;
				break;
			}
		}
		if(canMove)
			TETRIS.pieceY--;
	}

	/*Determines if piece needs to be speed up or not*/
	if(XY_AXIS[0] < JOY_X_THRES)
		TETRIS.speed = 40;
	else
		TETRIS.speed = 250;

	if(shortPressDetected)
	{
		if(TETRIS.rotation == getMaxRotation(TETRIS.currentShapeType) - 1 && canRotate(0))
		{
			TETRIS.rotation = 0;
		}
		else if(canRotate(TETRIS.rotation+1))
		{
			TETRIS.rotation++;
		}
		copyPiece(TETRIS.piece,TETRIS.currentShapeType,TETRIS.rotation);
		refresh();

		shortPressDetected = false;
	}
}

/**
 * @brief Displays the border on the OLED
 */
static void drawBorders(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	for (int i = MARGIN_SIDE-1; i <= GRID_WIDTH+MARGIN_SIDE; i++)
	{
		SSD1306_DrawPixel(i, MARGIN_TOP-1, 1);
		SSD1306_DrawPixel(i, MARGIN_TOP + GRID_HEIGHT, 1);
	}

	for (int i = MARGIN_TOP-1; i <= MARGIN_TOP + GRID_HEIGHT; i++)
	{
		SSD1306_DrawPixel(MARGIN_SIDE-1, i, 1);
		SSD1306_DrawPixel(MARGIN_SIDE+GRID_WIDTH - 4*GRID_BLOCK_SIZE, i, 1);
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
 * @breif Copies shape of piece based on type and rotation
 * @param piece Pointer to array, in which shape will be stored
 * @param type Type of shape to be copied
 * @param rotation Rotation of the piece
 */
static void copyPiece(char piece[2][4], uint8_t type, uint8_t rotation)
{
	switch(type)
	{
	case L_SHAPE:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_L_l[rotation][0][i];
			piece[1][i] = pieces_L_l[rotation][1][i];
		}
		break;
	case S_SHAPE_LEFT:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_S_l[rotation][0][i];
			piece[1][i] = pieces_S_l[rotation][1][i];
		}
		break;
	case S_SHAPE_RIGHT:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_S_r[rotation][0][i];
			piece[1][i] = pieces_S_r[rotation][1][i];
		}
		break;
	case SQUARE_SHAPE:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_Sq[0][0][i];  // Square doesn’t rotate
			piece[1][i] = pieces_Sq[0][1][i];
		}
		break;
	case T_SHAPE:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_T[rotation][0][i];
			piece[1][i] = pieces_T[rotation][1][i];
		}
		break;
	case LINE_SHAPE:
		for(int i = 0; i < 4; i++)
		{
			piece[0][i] = pieces_l[rotation][0][i];
			piece[1][i] = pieces_l[rotation][1][i];
		}
		break;
	}
}

/**
 * @brief Displays the tetris piece
 * @param type Type of the shape
 * @param rotation Rotation of the shape
 * @param x X-Coordinate of shape
 * @param y Y-Coordinate of shape
 */
void drawPiece(uint8_t type, uint8_t rotation, uint8_t x, uint8_t y)
{
  for(int i = 0; i < 4; i++)
	  SSD1306_DrawFilledRectangle(MARGIN_SIDE + GRID_BLOCK_SIZE*(x + TETRIS.piece[0][i]) , MARGIN_TOP + GRID_BLOCK_SIZE*(y + TETRIS.piece[1][i]), GRID_BLOCK_SIZE-2, GRID_BLOCK_SIZE-2, SSD1306_COLOR_WHITE);
}

/**
 * @brief Displays the next tetris piece
 */
static void drawNextPiece()
{
	char nPiece[2][4];							// Temporary array for next piece
	copyPiece(nPiece,TETRIS.nextShapeType,0);	// Copies next shape type into nPiece

	for(int i=0; i<4; i++)
	{
		SSD1306_DrawFilledRectangle(100 + GRID_BLOCK_SIZE * nPiece[0][i], 2 + GRID_BLOCK_SIZE * nPiece[1][i], GRID_BLOCK_SIZE - 2, GRID_BLOCK_SIZE - 2, SSD1306_COLOR_WHITE);
	}
}

/**
 * @brief Checks if piece will collide to bottom
 */
static bool ifCollision()
{
	for(int i = 0; i < 4; i++)
	{
		int y = TETRIS.pieceY + TETRIS.piece[1][i];
		int x = TETRIS.pieceX + TETRIS.piece[0][i] - 1;
		if(x < 0 || grid[x][y])
			return true;
	}
	return false;
}

/**
 * @brief Checks if line is full
 */
static void checkLines()
{
	bool full;

	for(int i=0;i<GRID_WIDTH/GRID_BLOCK_SIZE;i++)
	{
		full = true;
		for(int j=0;j<GRID_HEIGHT/GRID_BLOCK_SIZE;j++)
		{
			full = full && grid[i][j];
		}
		if(full)
		{
			breakLine(i);
			i--;
		}
	}
}

/**
 * @brief Increases score and adjusts the lines
 */
static void breakLine(uint8_t line)
{
	for(int x = line; x < GRID_WIDTH/GRID_BLOCK_SIZE-1; x++)
	{
		for(int y = 0; y < GRID_HEIGHT/GRID_BLOCK_SIZE; y++)
		{
			grid[x][y] = grid[x+1][y];
		}
	}
	for(int y = 0; y < GRID_HEIGHT/GRID_BLOCK_SIZE; y++)
	{
		grid[GRID_WIDTH/GRID_BLOCK_SIZE-1][y] = 0;
	}

	SSD1306_InvertDisplay(true);
	DELAY(50);
	SSD1306_InvertDisplay(false);

	GAME.score += GRID_HEIGHT/GRID_BLOCK_SIZE;
}

/**
 * @brief Checks for the maximum rotation types possible for shape
 * @retVal number of rotations valid
 */
static uint8_t getMaxRotation(uint8_t type)
{
	uint8_t numRotationPossible;

	switch(type)
	{
	case L_SHAPE:
		numRotationPossible = 4;
		break;
	case S_SHAPE_LEFT:
		numRotationPossible = 2;
		break;
	case S_SHAPE_RIGHT:
		numRotationPossible = 2;
		break;
	case SQUARE_SHAPE:
		numRotationPossible = 1;
		break;
	case T_SHAPE:
		numRotationPossible = 4;
		break;
	case LINE_SHAPE:
		numRotationPossible = 2;
		break;
	default:
		numRotationPossible = 0;
		break;
	}

	return numRotationPossible;
}


static bool canRotate(uint8_t rotation)
{
	char nPiece[2][4];
	uint8_t newY;

	copyPiece(nPiece,TETRIS.currentShapeType,rotation);

	for(int i=0;i<4;i++)	 // Loop through all 4 blocks of the piece
	{
		newY = TETRIS.pieceY + nPiece[1][i] + 1;  // Calculate new Y position of the block

		// Check if the new position is outside the game board or collides with an existing block
		if(newY >= GRID_HEIGHT/GRID_BLOCK_SIZE || grid[TETRIS.pieceX + nPiece[0][i]][newY])
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Displays the current frame of the game
 */
static void displayFrame(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	drawBorders();
	drawNextPiece();
	displayScore();

	SSD1306_UpdateScreen();
}

/**
 * @brief Draws the pieces that have reached end
 */
static void drawGrid()
{
	for(int i=0;i<GRID_WIDTH/GRID_BLOCK_SIZE;i++)
	{
		for(int j=0;j<GRID_HEIGHT/GRID_BLOCK_SIZE;j++)
		{
			if(grid[i][j])
				SSD1306_DrawFilledRectangle(MARGIN_SIDE + GRID_BLOCK_SIZE*i , MARGIN_TOP + GRID_BLOCK_SIZE*j, GRID_BLOCK_SIZE-2, GRID_BLOCK_SIZE-2, SSD1306_COLOR_WHITE);
		}
	}
}

/**
 * @brief Refreshes the game display
 */
static void refresh(void)
{
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	drawBorders();
	drawNextPiece();
	displayScore();

	drawGrid();
	drawPiece(TETRIS.currentShapeType, 0, TETRIS.pieceX, TETRIS.pieceY);

	SSD1306_UpdateScreen();
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

/**
 * @brief Generates a piece for tetris game
 */
static void generatePiece()
{
	TETRIS.currentShapeType = TETRIS.nextShapeType;		// Move next piece to current piece
	TETRIS.nextShapeType = rand()%SHAPE_TYPES;			// Pick a new random piece

	TETRIS.pieceY = (GRID_HEIGHT/GRID_BLOCK_SIZE)/2;	// Generates random y coordinate for piece(at top)
	TETRIS.pieceX = GRID_WIDTH/GRID_BLOCK_SIZE - 4;		// Generates random x coordinate for piece
	TETRIS.rotation = 0;

	copyPiece(TETRIS.piece, TETRIS.currentShapeType, TETRIS.rotation);
}
