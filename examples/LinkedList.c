#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linmath.h>
#include "wave.h"


void AddElement(struct Node  * list, struct Vertex * element)
{
	struct Node * new_node = (struct Node*) malloc(sizeof(struct Node));
	struct Node * last = NULL;
	new_node->vertex = malloc(sizeof(struct Vertex));
	GetLast(list, &last);
	new_node->vertex->x = element->x;
	new_node->vertex->y = element->y;
	new_node->vertex->z = element->z;
	new_node->vertex->r = element->r;
	new_node->vertex->g = element->g;
	new_node->vertex->b = element->b;
	new_node->previous = last;
	new_node->next = NULL;
	last->next = new_node;
}

void ToArray(struct Node list, struct Vertex ** array_pointer)
{
	int count = NumberOfElements(list);
	
	//Allocate array
	(*array_pointer) = malloc(count * sizeof(struct Vertex));
	
	//Copy values
	struct Node * current = NULL;
	GetFirst(&list, &current);
	int index = 0;
	while (current->vertex != NULL)
	{
		(*array_pointer)[index++] = *(current->vertex);
		if (current->next == NULL)
		{
			break;
		}
		else
		{
			current = current->next;
		}
	}
}

int NumberOfElements(struct Node list)
{
	//Get first element
	struct Node * first;
	GetFirst(&list, &first);

	//Count elements until last element
	int count = 1;
	struct Node * current = first;
	while (current->next != NULL)
	{
		current = current->next;
		count++;
	}

	return count;
}

void GetFirst(struct Node * node, struct Node ** first)
{
	struct Node * current = node;
	while (current->previous != NULL)
	{
		current = current->previous;
	}

	(*first) = (current);
}

void GetLast(struct Node * node, struct Node ** last)
{
	struct Node * current = node;
	while (current->next != NULL)
	{
		current = current->next;
	}

	(*last) = (current);
}

void PrintElements(struct Node node)
{
	OutputDebugString("Printing list elements:\r\n");
	char debugString[200] = { 0 };

	int element = 0;
	int count = NumberOfElements(node);
	struct Node * current = &node;
	while (current->vertex != NULL)
	{
		sprintf(debugString, "Element %d, Vertex: [X:%.1f] [Y:%.1f] [Z:%.1f] [R:%.1f] [G:%.1f] [B:%.1f]\r\n", element, current->vertex->x, current->vertex->y, current->vertex->z, current->vertex->r, current->vertex->g, current->vertex->b);
		OutputDebugString(debugString);
		element++;
		if (element >= count)
		{
			break;
		}
		
		if (current->next != NULL)
		{
			current = current->next;
		}
		else
		{
			break;
		}
	}
}