//******************************************************************************
//	snake_events.c  (07/13/2015)
//
//  Author:			Paul Roper, Brigham Young University
//  Revisions:		1.0		11/25/2012	RBX430-1
//
//******************************************************************************
//
#include "msp430.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "snake.h"
#include "snakelib.h"

extern volatile uint16 sys_event;			// pending events

volatile enum MODE game_mode;				// 0=idle, 1=play, 2=next
volatile uint16 score;						// current score
volatile uint16 seconds;					// time
volatile uint16 move_cnt;					// snake speed

volatile uint8 level;						// current level (1-4)
volatile uint8 direction;					// current move direction
volatile uint8 head;						// head index into snake array
volatile uint8 tail;						// tail index into snake array

volatile uint16 time_left;
volatile uint8 edible;
volatile uint8 snake_length;

extern const uint16 snake_text_image[];		// snake text image
extern const uint16 snake1_image[];			// snake image

SNAKE snake[MAX_SNAKE];						// snake segments

FOOD* food[10];								// food array
ROCK* rocks[3];								// rock array


FOOD* newFood(uint8 x, uint8 y, uint8 value, uint8 size);
ROCK* newRock(uint8 x, uint8 y, uint8 w, uint8 h);

void draw_circle_food(FOOD* food);
void draw_triangle_food(FOOD* food);
void draw_square_food(FOOD* food);
void draw_diamond_food(FOOD* food);
void draw_star_food(FOOD* food);
void draw_rock(ROCK* rock);

void createFoods(int max);
void createRocks(int max);



int checkRock(unsigned int row, unsigned int col){
	int valid = 0;
	unsigned int rockIndex = 0;

	do {
		if(rocks[rockIndex]->coordinates.point.x - 2 < col&& rocks[rockIndex]->coordinates.point.x + 2 > col){
			if( rocks[rockIndex]->coordinates.point.y - 2 < row&& rocks[rockIndex]->coordinates.point.y + 2 > row){
				return 0;		//if not blocking another rock
			} else {
				valid = 1;
			}
		} else {
			valid = 1;
		}
		++rockIndex;
	} while(rockIndex < 3);


	if( (row <= ROW(76) && row >= ROW(74)) && (col <= COL(67) && col >= COL(64))){	//if blocking border
		return 0;
	} else {
		valid = 1;
	}


	int i = 0;
	while(i < MAX_FOOD){	//if not blocking food
		if(food[i]->coordinates.point.x - 2 < col && food[i]->coordinates.point.x + 2 > col){
			if( food[i]->coordinates.point.y - 2 < row && food[i]->coordinates.point.y + 2 > row){
				return 0;
			} else {
				valid = 1; ++i;
			}
		} else {

			valid = 1;
			++i;
		}
	}

	i = 0;
	while(i < MAX_SNAKE){//if blocking snake
		if(snake[i].point.x == col && snake[i].point.y == row){
			return 0;
		} else {
			++i;
			valid = 1;
		}
	}
	return valid;
}

void createRocks(int max){
	unsigned int row, col;
	unsigned int i = 0;
	int valid;
	while(i < 3) {
		rocks[i] = NULL;	//set all to NULL
		++i;
	}
	i = 0;

	do {	//create rock in the quantity == max

		do {
			row = (rand() % 22) + 1;
			col = (rand() % 23) + 1;
			valid = checkRock(row,col);		//look for location to create rock ..If found a valid location then break
		} while(!valid);

		rocks[i] = newRock(col,row,5,5);
		draw_rock(rocks[i]);

	} while(++i < max);
}




//------------------------------------------------------------------------------
//-- move snake event ----------------------------------------------------------
//
int checkFood(unsigned int row, unsigned int col)
{
	int valid = 0;

	if( (row <= ROW(85) && row >= ROW(65)) && (col <= COL(75) && col >= COL(55)))
		return 0;

	unsigned int i = 0;
	while(i < MAX_FOOD){	//if this is where a food already is
		if(food[i]->coordinates.point.x == col && food[i]->coordinates.point.y == row){
			return 0;
		} else {
			++i;
			valid = 1;
		}
	}
	i = 0;

	while(i < MAX_SNAKE){
		if(snake[i].point.x == col && snake[i].point.y == row){	//check for locations of snake points
			return 0;
		} else {
			++i;
			valid = 1;
		}
	}
	return valid;
}


void createFoods(int max)
{
	unsigned int row, col, i;
	int typeOfFood;
	i=0;
	if(level > 1) {
		for(i = 0; i < 10; ++i) {
			if(food[i] != NULL) {
				free(food[i]);
				food[i] = NULL;
			}
		}
	}

	i = 0;
	do{

		do{
			row = (rand() % 22) + 1;
			col = (rand() % 23) + 1;
		} while(!checkFood(row,col));		//check for valid location to create food


		typeOfFood = rand() % 5;		//determine type of good randomly
		food[i] = newFood(col,row,level,2);

		switch(typeOfFood){
			case 0:
				draw_square_food(food[i]);
				break;
			case 1:
				draw_triangle_food(food[i]);
				break;
			case 2:
				draw_diamond_food(food[i]);
				break;
			case 3:
				draw_star_food(food[i]);
				break;
			case 4:
				draw_circle_food(food[i]);
				break;
			default:
				break;
		}

	} while(++i < max);

	while(max < MAX_FOOD){	//if not all foods are created then fill the empty slots with Null pointer
		food[i] = NULL;
		++max;
	}

}


int checkScore()
{
	int reached = 0;
	if(level == 1) {
		if(score == LEVEL1SCORE) {
			reached = 1;
		}
	}
	else if(level == 2) {
		if(score == LEVEL2SCORE) {
			reached = 1;
		}
	}
	else if(level == 3) {
		if(score == LEVEL3SCORE) {
			reached = 1;
		}
	}
	else if(level == 4) {
		if(score == LEVEL4SCORE) {
			reached = 1;

		}
	}
	if((reached == 1) && (level != 4)) {
		charge();
	}
	return reached;
}



int checkSnake()
{
	if(level > 0){
		unsigned int i = 0;
		for(i = tail; i < head; i = (i + 1) & (~MAX_SNAKE)){
			if(snake[head].xy == snake[i].xy) {
				return 1;
			}
			++i;
		}
		return 0;
	}
	return 0;
}


void MOVE_SNAKE_event(void)
{
	if (level > 0)
	{
		add_head();
		int r = ROW(snake[head].point.y);
		int c = COL(snake[head].point.x);
		lcd_point(c, r, PENTX);

		unsigned int i = 0;
		int eaten;


		do{
			if (snake[head].xy == food[i]->coordinates.xy) {		//iterates through each food		//if found one that is eaten then break out of loop
				score += food[i]->value;					//add the correct number for score

				free(food[i]); 								// free the space
				food[i] = NULL; 							// set the pointer to NULL

				beep();
				blink();
				eaten = 1;
			}
			else {
				eaten = 0;
			}
			if(eaten == 1) {
				if(level == 1) {
					unsigned int col, row;
					int typeOfFood;
					do {
						row = (rand() % 22) + 1;
						col = (rand() % 23) + 1;
					} while(!checkFood(row,col));


					typeOfFood = rand() % 5;					//determine type of good randomly
					food[i] = newFood(col,row,level,2);
					switch(typeOfFood){
						case 0:
							draw_square_food(food[i]);
							break;
						case 1:
							draw_triangle_food(food[i]);
							break;
						case 2:
							draw_diamond_food(food[i]);
							break;
						case 3:
							draw_star_food(food[i]);
							break;
						case 4:
							draw_circle_food(food[i]);
							break;
						default:
							break;
					}
				}
				break;
			}
			++i;
		} while(i < MAX_FOOD);


		if(eaten == 0) {
			delete_tail();		//delete tail if did not eat food in this move.
		}

		else {	//check score if eat something. move to next level if score is enough
			if(checkScore()) {
				sys_event |= NEXT_LEVEL;
				return;
			}
		}

		int hit = checkSnake();
		if(hit == 1) {		//check to see if the snake hits itself
			sys_event |= END_GAME;
			return;
		}


		if(level > 1) {		//check if snake hit a rock (only for level 2,3,4)
			i = 0;
			do {
				if (snake[head].point.x < rocks[i]->coordinates.point.x + 1 && snake[head].point.x > rocks[i]->coordinates.point.x - 1) {//if at a given coordinate
					if(snake[head].point.y < rocks[i]->coordinates.point.y + 1 && snake[head].point.y > rocks[i]->coordinates.point.y - 1){
						sys_event |= END_GAME;
						return;
					}
				}
				++i;
			} while(i < 3);
		}
	}
	return;

}




//------------------------------------------------------------------------------
//-- new game event ------------------------------------------------------------
//
void NEW_GAME_event(void)
{
	lcd_clear();						// clear lcd
	lcd_backlight(1);					// turn on backlight

	switch (game_mode)
	{
	case IDLE:
		// ***demo only***
		lcd_wordImage(snake1_image, (159-60)/2, 60, 1);
		lcd_wordImage(snake_text_image, (159-111)/2, 20, 1);
		lcd_diamond(COL(16), ROW(20), 2, 1);
		lcd_star(COL(17), ROW(20), 2, 1);
		lcd_circle(COL(18), ROW(20), 2, 1);
		lcd_square(COL(19), ROW(20), 2, 1);
		lcd_triangle(COL(20), ROW(20), 2, 1);
		// ***demo only***

		score = 0;
		level = 1;
		move_cnt = WDT_MOVE1;
	case NEXT:
		sys_event |= NEXT_LEVEL;
		level = 0;
		break;

	default:
		break;
	}
	sys_event |= START_LEVEL;

	return;
}


//------------------------------------------------------------------------------
//-- start level event -----------------------------------------------------------
//
void START_LEVEL_event(void)
{
	lcd_clear();
	game_mode = PLAY;
	new_snake(score, RIGHT);
	seconds = 0;

	switch(level) {
		case 1:
			move_cnt = WDT_MOVE1;
			createFoods(MAX_FOOD);
			break;
		case 2:
			move_cnt = WDT_MOVE2;
			createFoods(MAX_FOOD);
			createRocks(4);
			break;
		case 3:
			move_cnt = WDT_MOVE3;
			createFoods(MAX_FOOD);
			createRocks(3);
			break;
		case 4:
			move_cnt = WDT_MOVE4;
			createFoods(MAX_FOOD);
			createRocks(2);
			break;

		default:
			break;
	}
	return;
}


//------------------------------------------------------------------------------
//-- next level event -----------------------------------------------------------
//
void NEXT_LEVEL_event(void)
{
	lcd_clear();
	unsigned int i = 0;

	if(level > 1) {
		for(i = 0; i < 10; ++i) {
			if(food[i] != NULL) {
				free(food[i]);
				food[i] = NULL;
			}
		}
		for(i = 0; i < 3; ++i) {
			if(rocks[i] != NULL) {
				free(rocks[i]);
				rocks[i] = NULL;
			}
		}
	}

	if(level == 1)
	{
		level = 2;
		time_left = 30;
		sys_event |= START_LEVEL;
	}
	else if(level == 2)
	{
		level = 3;
		time_left = 45;
		sys_event |= START_LEVEL;
	}
	else if(level == 3)
	{
		level = 4;
		time_left = 60;
		sys_event |= START_LEVEL;
	}
	else if(level == 4)
	{
		sys_event = END_GAME;
		return;
	}
	else if(level == 0)
	{
		level = 1;
		time_left = 30;
		sys_event |= START_LEVEL;
	}

	lcd_cursor(ROW(0)-7, COL(23)+2);
	lcd_printf("SCORE:      LEVEL:      ");	// Display new level.
	lcd_cursor(ROW(5), COL(23)+2);
	lcd_printf("\t%d",score);
	lcd_cursor(ROW(16)+1, COL(23)+2);
	lcd_printf("\t%d",level);
	new_snake(score, RIGHT);

	return;
}



//------------------------------------------------------------------------------
//-- end game event -------------------------------------------------------------
//
void END_GAME_event(void)
{
	lcd_clear();


	unsigned int i = 0;
	for(i = 0; i < 10; ++i) {
		if(food[i] != NULL) {
			free(food[i]);
			food[i] = NULL;
		}
	}
	for(i = 0; i < 3; ++i) {
		if(rocks[i] != NULL) {
			free(rocks[i]);
			rocks[i] = NULL;
		}
	}

	lcd_cursor(55, 65);
	if(score >= LEVEL4SCORE){
		lcd_wordImage(snake_text_image, (159-111)/2-10, 45, 1);
		imperial_march();
		lcd_cursor(COL(5)+2, 25);
		lcd_printf("\bWINNER!");
		lcd_cursor(COL(4)+2, 3);
		lcd_printf("\bSCORE:%d", score);
	} else {
		rasberry();
		printf("GAME OVER");
		lcd_cursor(55, 40);
		printf("SCORE:%d", score);
	}
	game_mode = IDLE;
	return;

}


//------------------------------------------------------------------------------
//-- switch #1 event -----------------------------------------------------------
//
void SWITCH_1_event(void)
{
	switch (game_mode)
	{
		case IDLE:						// NEW_GAME will know what to do.
			sys_event |= NEW_GAME;
			break;
		case PLAY:						// Move in direction.
			if (direction != LEFT)
			{
				if (snake[head].point.x < X_MAX)
				{
					direction = RIGHT;
					sys_event |= MOVE_SNAKE;
				}
			}
			break;
		case NEXT:						// start next level.
			sys_event |= NEXT_LEVEL;
			break;
		case EOG:						// You Won!! goto king snake.
			sys_event |= NEW_GAME;
			game_mode = PLAY;
			//game_mode = SETUP;
			break;
	}
	return;
}



//------------------------------------------------------------------------------
//-- switch #2 event -----------------------------------------------------------
//
void SWITCH_2_event(void)
{
	switch (game_mode)
	{

	case NEXT:						// start next level.
		sys_event |= NEXT_LEVEL;
		break;
	case PLAY:						// Move in direction.
		if (direction != UP)
		{
			if (snake[head].point.y < Y_MAX)
			{
				direction = DOWN;
				sys_event |= MOVE_SNAKE;
			}
		}
	default:
		break;

	}
}


//------------------------------------------------------------------------------
//-- switch #3 event -----------------------------------------------------------
//
void SWITCH_3_event(void)
{

	switch (game_mode)
	{
		case NEXT:						// start next level.
			sys_event |= NEXT_LEVEL;
			break;
		case PLAY:						// Move in direction.
			if (direction != DOWN)
						{
							if (snake[head].point.y > XY_MIN-1)
							{
								direction = UP;
					sys_event |= MOVE_SNAKE;
				}
			}
		default:
			break;

	}
}


//------------------------------------------------------------------------------
//-- switch #4 event -----------------------------------------------------------
//
void SWITCH_4_event(void)
{
	switch (game_mode)
	{
		case NEXT:						// start next level.
			sys_event |= NEXT_LEVEL;
			break;
		case PLAY:						// Move in direction.
			if (direction != RIGHT)
			{
				if (snake[head].point.x > XY_MIN-1)
				{
					direction = LEFT;
					sys_event |= MOVE_SNAKE;
				}
			}
		default:
			break;
	}
	return;
}

//------------------------------------------------------------------------------
//-- update LCD event -----------------------------------------------------------
//
void LCD_UPDATE_event(void)
{
	if(level > 0) {
		if(game_mode == PLAY) {
			lcd_cursor(ROW(20), COL(23)+2);					// Write current time and score.
			if ((time_left - seconds) < 10)
				lcd_printf("\t%d:0%d",(time_left - seconds) / 60,(time_left - seconds) % 60);//,score,level, seconds);
			else
				lcd_printf("\t%d:%d",(time_left - seconds) / 60,(time_left - seconds) % 60);//,score,level, seconds);

			if ((time_left - seconds) == 0) {
				sys_event |= END_GAME;
				lcd_cursor(COL(0), ROW(0));
				lcd_printf("Time");
			}

			lcd_cursor(COL(0), ROW(23));
			printf("Score:");
			printf("%d", score);

			lcd_cursor(COL(10), ROW(23));
			printf("Level:");
			printf("%d", level);

			lcd_cursor(COL(0), ROW(0));
			printf("LEFT");
			lcd_cursor(COL(7), ROW(0));
			printf("UP");
			lcd_cursor(COL(13), ROW(0));
			printf("DOWN");
			lcd_cursor(COL(19), ROW(0));
			printf("RIGHT");
		}

	}

}



ROCK* newRock(uint8 x, uint8 y, uint8 w, uint8 h)
{  ROCK* rock = (ROCK*)malloc(sizeof(ROCK));
   rock->coordinates.point.x = x;
   rock->coordinates.point.y = y;
   rock->width = w;
   rock->height = h;
   return rock;
}


FOOD* newFood(uint8 x, uint8 y, uint8 value, uint8 size)
{  FOOD* food = (FOOD*)malloc(sizeof(FOOD));
   food->coordinates.point.x = x;
   food->coordinates.point.y = y;
   food->value = value;
   food->size = size;
   return food;
}


void draw_circle_food(FOOD* food)
{  lcd_circle(COL(food->coordinates.point.x),
              ROW(food->coordinates.point.y),
              food->size, 1);
}
void draw_triangle_food(FOOD* food)
{  lcd_triangle(COL(food->coordinates.point.x),
              ROW(food->coordinates.point.y),
              food->size, 1);
}
void draw_square_food(FOOD* food)
{  lcd_square(COL(food->coordinates.point.x),
              ROW(food->coordinates.point.y),
              food->size, 1);
}
void draw_diamond_food(FOOD* food)
{  lcd_diamond(COL(food->coordinates.point.x),
              ROW(food->coordinates.point.y),
              food->size, 1);
}
void draw_star_food(FOOD* food)
{  lcd_star(COL(food->coordinates.point.x),
              ROW(food->coordinates.point.y),
              food->size, 1);
}
void draw_rock(ROCK* rock)
{  lcd_rectangle(COL(rock->coordinates.point.x),
              ROW(rock->coordinates.point.y),
              rock->width,rock->height, 3);
}

