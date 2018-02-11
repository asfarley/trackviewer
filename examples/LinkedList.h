#pragma once

struct Node {
	struct Node * previous;
	struct Node * next;
	struct Vertex * vertex;
};

void AddElement(struct Node * list, struct Vertex * element);

void ToArray(struct Node list, struct Vertex ** array_pointer);

int NumberOfElements(struct Node list);

void GetFirst(struct Node * node, struct Node ** first);

void GetLast(struct Node * node, struct Node ** last);

void PrintElements(struct Node node);