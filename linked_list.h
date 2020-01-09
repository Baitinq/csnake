#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

typedef struct Node
{
	int x, y;
	struct Node *next, *prev;
} Node;

typedef struct LinkedList
{
	Node* head, *tail;
} LinkedList;

Node *initialise_node(void);
void free_node(Node *);
LinkedList *initialise_linked_list(void);
void free_linked_list(LinkedList *);
void append_linked_list(LinkedList *, int, int);
void remove_tail_linked_list(LinkedList *);
void print_linked_list(LinkedList *, void (*)(void *));


#endif

