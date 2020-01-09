#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

Node *initialise_node()
{
	Node* newNode = malloc(sizeof(Node));
	
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

void free_node(Node* node)
{
	if(node)
		free(node);
	return;
}

LinkedList *initialise_linked_list()
{
	LinkedList* newList = malloc(sizeof(LinkedList));
	
	newList->head = NULL;
	newList->tail = NULL;

	return newList;
}

void free_linked_list(LinkedList* list)
{
	Node* element;

	while(list->head)
	{
		element = list->head->next;
		free_node(list->head);
		list->head = element;
	}
	free(list);
}

void append_linked_list(LinkedList* list, int x, int y)
{
	Node* newNode = initialise_node();
	
	newNode->x = x;
	newNode->y = y;

	newNode->prev = list->tail;

	if(list->tail)
		list->tail->next = newNode;
	list->tail = newNode;
	if(!list->head)
		list->head = newNode;
}

void remove_tail_linked_list(LinkedList* list)
{
	Node* element;

	if(!list->tail)
		return;
	element = list->tail->prev;
	free_node(list->tail);
	list->tail = element;
	if(list->tail)
		list->tail->next = NULL;
	else
		list->head = NULL;
}

