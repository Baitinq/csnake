#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <ncurses.h>
#include "linked_list.h"

void playGame(LinkedList* snake, int* apple, int* stats);
void drawSnake(LinkedList* snake, int* apple);
void init_snake(LinkedList* snake);
int check_collision(LinkedList* snake, int* apple);
void move_left(LinkedList* snake, int* new, int* status);
void move_right(LinkedList* snake, int* new, int* status);
void move_up(LinkedList* snake, int* new, int* status);
void move_down(LinkedList* snake, int* new, int* status);
void follow(int* tmp, int* prev, LinkedList* snake, int* new);
void newApple(LinkedList* snake, int* apple);

int fps = 3;

int main()
{
	WINDOW* windw;
	LinkedList* snake;
	int stats[3], apple[2];
	int duration, score, win;
	
	srand(time(NULL));
	windw = initscr();
	clear();
	noecho();
	//cbreak();
	nodelay(windw, true);

	snake = initialise_linked_list();
	apple[0] = 60;
	apple[1] = 15;

	/* add initial nodes */
	init_snake(snake);

	playGame(snake, apple, stats);

	duration = stats[0];
	score = stats[1];
	win = stats[2];
	
	endwin();

	if(win)
		printf("You won! %d\n", win);
	else
		printf("You lost!\n");
	
	printf("Time: %d\n", duration);
	printf("Score: %d\n", score);

	return 0;
}

void playGame(LinkedList* snake, int* apple, int* stats)
{
	int start = (int)time(NULL);
	int score, win, new[0];
	char c = 'a';
	int status[0];
	score = win = *new = 0;
	move_left(snake, new, status);
	while(1)
	{
		c = getch();
		switch(c)
		{
			case 'q':
				return;
			case 'w':
				move_up(snake, new, status);
				break;
			case 'a':
				move_left(snake, new, status);
				break;
			case 's':
				move_down(snake, new, status);
				break;
			case 'd':
				move_right(snake, new, status);
				break;
			default:
				break;
		}

		switch(check_collision(snake, apple))
		{
			//apple
			case 1:
				score++;
				newApple(snake, apple);
				//new node
				*new = 1;
				break;
			//self
			case 2:
				win = 0;
				return;
				break;
			//wall
			case 3:
				win = 0;
				return;
				break;
			default:
				break;
		}
		clear();
		drawSnake(snake, apple);
                refresh();
		usleep(1000000 / fps);
	}
	
	// time
	*(stats + 0) = (int)time(NULL) - start;
	// score
	*(stats + 1) = score;
	// win
	*(stats + 2) = win;
}

void drawSnake(LinkedList* snake, int* apple)
{
	Node* element;
	
	mvaddstr(snake->head->y, snake->head->x, "o");

	element = snake->head->next;
	while(element)
	{
		mvaddstr(element->y, element->x, "*");
		element = element->next;
	}

	mvaddstr(apple[1], apple[0], "=");
}

void init_snake(LinkedList* snake)
{
	int midx = COLS / 2; 
	int midy = LINES / 2;
	
	append_linked_list(snake, midx - 2, midy);
	append_linked_list(snake, midx - 1, midy);
	append_linked_list(snake, midx, midy);
	append_linked_list(snake, midx + 1, midy);
	append_linked_list(snake, midx - 2, midy);
}

int check_collision(LinkedList* snake, int* apple)
{
	Node* element;

	/* check collision with apple */
        if(snake->head->x == apple[0] && snake->head->y == apple[1])
                return 1;

	/* check collision with self */
	element = snake->head->next;
	while(element)
	{
		if(snake->head->x == element->x && snake->head->y == element->y)
			return 2;
		element = element->next;
	}

	/* check collision with wall */
	if((snake->head->y == 0 || snake->head->y == LINES) || (snake->head->x == 0 || snake->head->x == COLS))
		return 3;

	/* no collision */
	return 0;
}

void move_left(LinkedList* snake, int* new, int* status)
{
	/* You cant move left if youre going right */
	if(*status == 1)
		return;
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->x = snake->head->x - 1;

        follow(tmp, prev, snake, new);
	*status = 0;
}

void move_right(LinkedList* snake, int* new, int* status)
{
	if(*status == 0)
		return;
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->x = snake->head->x + 1;

        follow(tmp, prev, snake, new);
	*status = 1;
}

void move_up(LinkedList* snake, int* new, int* status)
{
	if(*status == 3)
		return;
        int tmp[2];
	int prev[2];

        *(prev + 0) = snake->head->x;
	*(prev + 1) = snake->head->y;
        snake->head->y = snake->head->y - 1;
	
	follow(tmp, prev, snake, new);
	*status = 2;
}

void move_down(LinkedList* snake, int* new, int* status)
{
	if(*status == 2)
		return;
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->y = snake->head->y + 1;

        follow(tmp, prev, snake, new);
	*status = 3;
}

void follow(int* tmp, int* prev, LinkedList* snake, int* new)
{
        Node* element;
        element = snake->head->next;
        while(element)
        {
                *(tmp + 0) = element->x;
                *(tmp + 1) = element->y;
		
                element->x = *(prev + 0);
                element->y = *(prev + 1);

                *(prev + 0) = *(tmp + 0);
                *(prev + 1) = *(tmp + 1);

                element = element->next;
        }

	if(*new)
	{
		append_linked_list(snake,  *(prev + 0),  *(prev + 1));
		*new = 0;
	}
}

void newApple(LinkedList* snake, int* apple)
{
	Node* element;

	apple[0] = (rand() % (COLS - 0 + 1)) + 0;
        apple[1] = (rand() % (LINES - 0 + 1)) + 0;
	
	element = snake->head;
	if(element)
	{
		if(element->x == apple[0] && element->y == apple[1])
			newApple(snake, apple);
		element = element->next;
	}
}




