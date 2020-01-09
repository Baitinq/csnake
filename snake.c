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
void move_left(LinkedList* snake);
void move_right(LinkedList* snake);
void move_up(LinkedList* snake);
void move_down(LinkedList* snake);
void follow(int* tmp, int* prev, LinkedList* snake);

int fps = 60;

int main()
{
	WINDOW* windw;
	LinkedList* snake;
	int stats[3], apple[2];
	int time, score, win;
	
	windw = initscr();
	clear();
	noecho();
	cbreak();

	snake = initialise_linked_list();
	apple[0] = 20;
	apple[1] = 10;
		//= { 1, 2};
		
	/* add initial nodes */
	init_snake(snake);

	playGame(snake, apple, stats);
	
	time = stats[0];
	score = stats[1];
	win = stats[2];

	getch();			/* Wait for user input */
	endwin();	
	printf("Time: %d\n", time);
	printf("Score: %d\n", score);

	return 0;
}

void playGame(LinkedList* snake, int* apple, int* stats)
{
	int start = (int)time(NULL);
	int score = 0;
	int win = 0;
	int i = 0;
	while(1)
	{
		clear();
		printw("Update %d", i);
		if(i < 7)
			move_up(snake);
		else if(i < 12)
			move_left(snake);
		else if(i < 17)
			move_down(snake);
		else
			move_right(snake);
	
		drawSnake(snake, apple);
                refresh();
		switch(check_collision(snake, apple))
		{
			//apple
			case 1:
				score++;
				//new apple
				//new node
				append_linked_list(snake, snake->tail->x - 1, snake->tail->y);
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
		usleep(50000);
		//usleep(1000000 / fps);
		i++;
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
	
	element = snake->head;
	while(element)
	{
		mvaddstr(element->y, element->x, "*");
		element = element->next;
	}

	mvaddstr(apple[0], apple[1], "=");
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
	//0 no collision
	//1 collision with apple
	//2 collision with self
	//3 collision with wall
	
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

void move_left(LinkedList* snake)
{
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->x = snake->head->x - 1;

        follow(tmp, prev, snake);
}

void move_right(LinkedList* snake)
{
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->x = snake->head->x + 1;

        follow(tmp, prev, snake);
}

void move_up(LinkedList* snake)
{
        int tmp[2];
	int prev[2];

        *(prev + 0) = snake->head->x;
	*(prev + 1) = snake->head->y;
        snake->head->y = snake->head->y - 1;
	
	follow(tmp, prev, snake);
}

void move_down(LinkedList* snake)
{
	int tmp[2];
        int prev[2];

        *(prev + 0) = snake->head->x;
        *(prev + 1) = snake->head->y;
        snake->head->y = snake->head->y + 1;

        follow(tmp, prev, snake);
}

void follow(int* tmp, int* prev, LinkedList* snake)
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
}




