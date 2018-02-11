#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linmath.h>
#include "wave.h"
#include "LinkedList.h"

void ParseTextfile(FILE * fp, struct Node ** linked_list)
{
	//Open file
	char line[256];
	char debugString[100] = { 0 };
	int len = 0;
	struct Node * current = malloc(sizeof(struct Node));
	(*linked_list) = current;

	int created_first_element = 0;

	while (fgets(line, sizeof(line), fp))
	{
		len = strlen(line);
		OutputDebugString("Parsing line\r\n");
		//OutputDebugString(line);

		char * pch;
		pch = strtok(line, " ");
		int index = 0;

		if (len < 5)
		{
			break;
		}

		struct Vertex * new_vertex = malloc(sizeof(struct Vertex));

		while (pch != NULL)
		{
			int val = atoi(pch);

			switch (index)
			{
			case 0:
				new_vertex->x = (float) val;
				break;
			case 1: 
				new_vertex->y = (float)val;
				break;
			case 2: 
				new_vertex->z = (float)val;
				break;
			case 3: 
				new_vertex->r = val / 255.0;
				break;
			case 4: 
				new_vertex->g = val / 255.0;
				break;
			case 5: 
				new_vertex->b = val / 255.0;
				break;
			default:
				break;
			}

			pch = strtok(NULL, " ");
			index++;
		}

		sprintf(debugString, "new_vertex[%d]: %.1f, %.1f, %.1f, %.1f, %.1f, %.1f \r\n", index, new_vertex->x, new_vertex->y, new_vertex->z, new_vertex->r, new_vertex->g, new_vertex->b);
		OutputDebugString(debugString);

		if (created_first_element)
		{
			AddElement((*linked_list), new_vertex);
		}
		else
		{
			(*linked_list) = malloc(sizeof(struct Node));
			(*linked_list)->previous = NULL;
			(*linked_list)->next = NULL;
			(*linked_list)->vertex = new_vertex;
			created_first_element = 1;
		}
	}
	//For each line:
	//Parse into x,y,z,r,g,b
	//Allocate new vertex

	fclose(fp);
}