#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>

int SPEED = 100000;

struct point {
		int x, y;
	};
	
void moveSnake(int ch, point* snake);

void printSnake(point* snake);

int validateSnake(point* food, point* snake, int winHeight, int winWidth, int ch, int &score);
	
void drawBorder(int winHeight, int winWidth);

void updSnake(point* snake, int ch);

void drawFood(int winHeight, int winWidth, point* food, int &foodCreate);

void drawScore(int winHeight, int score);
	
int main()
{
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
	point *snake = new point[1];
	snake[0].x = (x_max-2)/2;
	snake[0].y = (y_max-3)/2;
	/*---------SNAKE INSTANTIATION---------*/
	
	/*----------FOOD INSTANTIATION---------*/
	point *food = new point[5];
	for(int i = 0; i < 5; i++)
	{
		food[i].x = (rand()%(x_max-1)+1);
		food[i].y = (rand()%(y_max-2)+1);
	}
	/*----------FOOD INSTANTIATION---------*/
	
	int ch, temp_ch;
	nodelay(stdscr, TRUE);
	int foodCreate = 0;
	int score = 0;
	
	while(ch != 27){
		ch = getch();
		if(ch == ERR) { ch = temp_ch; }
		temp_ch = ch;
		clear();
		drawBorder(y_max, x_max);
		moveSnake(ch, snake);
		ch = validateSnake(food, snake, y_max, x_max, ch, score);
		drawScore(y_max, score);
		refresh();
		foodCreate++;
		drawFood(y_max, x_max, food, foodCreate);
		printSnake(snake);
		refresh();
		usleep(SPEED);
	}
	
	clear();
	mvprintw(y_max/2, (x_max-9)/2, "GAME OVER");
	delete [] snake;
	refresh();
	getch();
	endwin();
	nocbreak();
	return 0;
}

void moveSnake(int ch, point* snake)
{
	if(ch == KEY_LEFT)
		snake[0].x -= 1;
	else if(ch == KEY_RIGHT)
		snake[0].x += 1;
	else if(ch == KEY_UP)
		snake[0].y -= 1;
	else if(ch == KEY_DOWN)
		snake[0].y += 1;
}

void printSnake(point* snake)
{
	mvprintw(snake[0].y, snake[0].x, "S");
}

int validateSnake(point* food, point* snake, int winHeight, int winWidth, int ch, int &score)
{
	int x,y;
	x = snake[0].x;
	y = snake[0].y;
	for(int i = 0; i < 5; i++)
	{
		if(food[i].x == x && food[i].y == y)
		{
			score += 10;
			food[i].x = (rand()%(winWidth-2)+1);
			food[i].y = (rand()%(winHeight-3)+1);
			mvprintw(food[i].y, food[i].x, "F");
		}
	}
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

void updSnake(point* snake, int ch){
	
}

void drawFood(int winHeight, int winWidth, point* food, int &foodCreate){
	/*create new food*/
	if(foodCreate == 30)
	{
		foodCreate = 0;
		for(int i = 0; i < 4; i++){
			food[i].x = food[i + 1].x;
			food[i].y = food[i + 1].y;
		}
		food[4].x = (rand()%(winWidth-2)+1);
		food[4].y = (rand()%(winHeight-3)+1);
	}	
	/*print food*/
	for(int i = 0; i < 5; i++)
	{
		mvprintw(food[i].y, food[i].x, "F");
	}
	
}

void drawScore(int winHeight, int score)
{
	mvprintw(winHeight-1, 0, "The score is %d", score);
}