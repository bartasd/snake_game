#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <bits/stdc++.h>

struct point {
		int x, y;
	};
	
void moveSnake(int ch, point* snake, int len);

void printSnake(point* snake, int len);

int validateSnake(point* food, point* snake, point* poison, int winHeight, int winWidth, int ch, int &score, int &len);
	
void drawBorder(int winHeight, int winWidth);

void updSnake(point* snake, int &len, int ch, int score, int &tempScore, int &lvl, int &SPEED, int difSPEED);

void drawFood(int winHeight, int winWidth, point* food, point* poison, int &foodCreate, int &foodSpeed);

void drawScore(int winHeight, int score, int lvl, time_t time);

bool validate_direction(int ch, int direction);

void initPointObj(point* ob, int y_max, int x_max);
	
int main()
{
	int SPEED = 100000;
	int difSPEED = 10000;
	
	initscr();				// INITIATE NCURSES ENVIRONTMENT
	cbreak();				// 
	keypad(stdscr, TRUE);	// TO GAIN ACCESS TO ALL KEYS
	noecho();				// NOT TO SHOW PRESSED KEYS
	curs_set(0);			// TO HIDE THE CURSOR
	
	/*---------DISPLAY---------*/
	int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);
	/*---------DISPLAY---------*/
	
	/*---------SNAKE INSTANTIATION---------*/
	point *snake = new point[100];
	snake[0].x = (x_max-2)/2;
	snake[0].y = (y_max-3)/2;
	int snakeLen = 1;
	/*---------SNAKE INSTANTIATION---------*/
	
	/*----------FOOD INSTANTIATION---------*/
	point *food = new point[5];
	initPointObj(food, y_max, x_max);
	/*----------FOOD INSTANTIATION---------*/
	
	/*----------POISON INSTANTIATION---------*/
	point *poison = new point[5];
	initPointObj(poison, y_max, x_max);
	/*----------POISON INSTANTIATION---------*/
	
	int ch, direction;			// Represents the current input and active snake direction
	nodelay(stdscr, TRUE);
	int foodCreate = 0;
	int foodSpeed = 100;
	int score = 0;
	int tempScore = score;
	int lvl = 1;
	time_t start, end, now;
	time_t t;
	
	time(&start);
	
	while(ch != 27){
		ch = getch();
		if(ch == ERR)  { ch = direction; }
		else if (validate_direction(ch, direction)) 
		{ 
			direction = ch;		// to avoid snake direction inside itself...
		}
		clear();
		moveSnake(ch, snake, snakeLen);
		drawBorder(y_max, x_max);
		ch = validateSnake(food, snake, poison, y_max, x_max, ch, score, snakeLen);
		time(&now);
		t = double(now - start);
		drawScore(y_max, score, lvl, t);
		refresh();
		foodCreate++;
		drawFood(y_max, x_max, food, poison, foodCreate, foodSpeed);
		updSnake(snake, snakeLen, ch, score, tempScore, lvl, SPEED, difSPEED);
		printSnake(snake, snakeLen);
		refresh();
		usleep(SPEED);
	}
	
	time(&end);
	
	nodelay(stdscr, FALSE);
	clear();
	mvprintw(y_max/2-1, (x_max-9)/2, "GAME OVER");
	mvprintw(y_max/2, (x_max-18)/2, "Time spent: %d sec", end-start);
	
	delete [] snake;
	refresh();
	getch();
	endwin();
	nocbreak();
	return 0;
}

bool validate_direction(int ch, int direction)
{
	if(ch == KEY_LEFT && direction == KEY_RIGHT)
		return false;
	else if(ch == KEY_RIGHT && direction == KEY_LEFT)
		return false;
	else if(ch == KEY_UP && direction == KEY_DOWN)
		return false;
	else if(ch == KEY_DOWN && direction == KEY_UP)
		return false;
	else
		return true;
}

void moveSnake(int ch, point* snake, int len)
{
	for(int i = len-1; i >= 0; i--)
	{
		if(i == 0)						// move the head of the snake
		{
			if(ch == KEY_LEFT)
				snake[i].x -= 1;
			else if(ch == KEY_RIGHT)
				snake[i].x += 1;
			else if(ch == KEY_UP)
				snake[i].y -= 1;
			else if(ch == KEY_DOWN)
				snake[i].y += 1;
		}
		else							// and make other snake parts follow
		{
			snake[i].x = snake[i-1].x;
			snake[i].y = snake[i-1].y;
		}
	}		
}

void printSnake(point* snake, int len)
{
	// simply print the parts of the snake
	for(int i = 0; i < len; i++)
	{
		mvprintw(snake[i].y, snake[i].x, "S");
	}
}

int validateSnake(point* food, point* snake, point* poison, int winHeight, int winWidth, int ch, int &score, int &len)
{
	int x,y;
	x = snake[0].x;
	y = snake[0].y;
	
	// check for fruit and snake-head collisions
	for(int i = 0; i < 5; i++)
	{
		if(food[i].x == x && food[i].y == y)
		{
			score += 10;
			// re-appear the FOOD in other place
			food[i].x = (rand()%(winWidth-2)+1);
			food[i].y = (rand()%(winHeight-3)+1);
			mvprintw(food[i].y, food[i].x, "F");
		}
	}
	
	// check for poison and snake-head collisions
	for(int i = 0; i < 5; i++)
	{
		if(poison[i].x == x && poison[i].y == y)
		{
			score -= 20;
			if(score < 0)
				return 27;
			len /= 2;
			// re-appear the POISON in other place
			poison[i].x = (rand()%(winWidth-2)+1);
			poison[i].y = (rand()%(winHeight-3)+1);
			mvprintw(poison[i].y, poison[i].x, "P");
		}
	}
	
	// check for snake-head and border collisions
	if(x == 0 || x == winWidth - 1 || y == 0 || y == winHeight - 2 )
		return 27;
	else
		return ch;
}

void drawBorder(int winHeight, int winWidth){
	for(int i = 0; i < winWidth; i++) { 
		mvprintw(0, i, "X");
		mvprintw(winHeight-2, i, "X");
	}
	for(int i = 1; i < winHeight-2; i++) { 
		mvprintw(i, 0, "X");
		mvprintw(i, winWidth-1, "X");
	}
}	

void updSnake(point* snake, int &len, int ch, int score, int &tempScore, int &lvl, int &SPEED, int difSPEED ){
	// make snake longer
	if(score != tempScore)
	{
		tempScore = score;
		len++;
		if(score%50 == 0 && SPEED > 10000)
		{
			lvl++;
			SPEED -= difSPEED;
		}
		if(ch == KEY_LEFT)
		{
			snake[len-1].x = snake[len-2].x + 1;
			snake[len-1].y = snake[len-2].y;
		}
		else if(ch == KEY_RIGHT)
		{
			snake[len-1].x = snake[len-2].x - 1;
			snake[len-1].y = snake[len-2].y;
		}
		else if(ch == KEY_UP)
		{
			snake[len-1].x = snake[len-2].x;
			snake[len-1].y = snake[len-2].y+1;	
		}
		else if(ch == KEY_DOWN)
		{
			snake[len-1].x = snake[len-2].x;
			snake[len-1].y = snake[len-2].y-1;
		}
	}
	// make snake shorter
	for(int i = 3; i < len; i++)
	{
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			len = i;
			SPEED += 30000;
			lvl--;
		}
	}	
}

void drawFood(int winHeight, int winWidth, point* food, point* poison, int &foodCreate, int &foodSpeed){
	/*create new food*/
	if(foodCreate == foodSpeed)	
	{
		foodCreate = 0;
		if(foodSpeed != 25) { foodSpeed -= 5; }
		for(int i = 0; i < 4; i++){
			food[i].x = food[i + 1].x;
			food[i].y = food[i + 1].y;
			poison[i].x = poison[i + 1].x;
			poison[i].y = poison[i + 1].y;
		}
		food[4].x = (rand()%(winWidth-2)+1);
		food[4].y = (rand()%(winHeight-3)+1);
		poison[4].x = (rand()%(winWidth-2)+1);
		poison[4].y = (rand()%(winHeight-3)+1);
	}	
	/*print food & poison*/
	for(int i = 0; i < 5; i++)
	{
		mvprintw(food[i].y, food[i].x, "F");
		mvprintw(poison[i].y, poison[i].x, "P");
	}
}

void drawScore(int winHeight, int score, int lvl, time_t time)
{
	mvprintw(winHeight-1, 0, "%d. Level. The score is %d. Time elapsed: %d",lvl, score, time);
}

void initPointObj(point* ob, int y_max, int x_max)
{
	for(int i = 0; i < 5; i++)
	{
		ob[i].x = (rand()%(x_max-1)+1);
		ob[i].y = (rand()%(y_max-2)+1);
	}
}