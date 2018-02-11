/*****************************************************************************
 * Wave Simulation in OpenGL
 * (C) 2002 Jakob Thomsen
 * http://home.in.tum.de/~thomsen
 * Modified for GLFW by Sylvain Hellegouarch - sh@programmationworld.com
 * Modified for variable frame rate by Marcus Geelnard
 * 2003-Jan-31: Minor cleanups and speedups / MG
 * 2010-10-24: Formatting and cleanup - Camilla Löwy
 *****************************************************************************/

#if defined(_MSC_VER)
 // Make MS math.h define M_PI
 #define _USE_MATH_DEFINES
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linmath.h>
#include "wave.h"
#include "LinkedList.h"

#define INTERFRAME_HEIGHT_SPACING_PX 1

// Maximum delta T to allow for differential calculations
#define MAX_DELTA_T 0.01

// Animation speed (10.0 looks good)
#define ANIMATION_SPEED 10.0

GLfloat alpha = 210.f, beta = -70.f;
GLfloat zoom = 2.f;

double cursorX;
double cursorY;



#define GRIDW 5
#define GRIDH 5
#define GRIDD 2
#define VERTEXNUM (GRIDW*GRIDH*GRIDD)

GLuint quad[VERTEXNUM];
struct Vertex vertex[VERTEXNUM];
struct Node linked_list;
struct Vertex * vertex_array = NULL;
int vertex_array_length = 0;

/* The grid will look like this:
 *
 *      3   4   5
 *      *---*---*
 *      |   |   |
 *      | 0 | 1 |
 *      |   |   |
 *      *---*---*
 *      0   1   2
 */


void update_vertex_array()
{
	ToArray(linked_list, &vertex_array);
	vertex_array_length = NumberOfElements(linked_list);
	glVertexPointer(3, GL_FLOAT, sizeof(struct Vertex), vertex_array);
	glColorPointer(3, GL_FLOAT, sizeof(struct Vertex), &(vertex_array[0].r)); // Pointer to the first color
}


//========================================================================
// Initialize grid geometry
//========================================================================

void init_vertices(void)
{
    int x, y, z, p;

    // Place the vertices in a grid
    for (y = 0;  y < GRIDH;  y++)
    {
        for (x = 0;  x < GRIDW;  x++)
        {
			for (z = 0; z < GRIDD; z++)
			{
				p = (y * GRIDW * GRIDD) + (x * GRIDD) + z;

				vertex[p].x = (GLfloat) (x - GRIDW / 2) / (GLfloat) (GRIDW / 2);
				vertex[p].y = (GLfloat) (y - GRIDH / 2) / (GLfloat) (GRIDH / 2);
				vertex[p].z = INTERFRAME_HEIGHT_SPACING_PX*z;
				//vertex[p].z = 0;

				if ((x % 4 < 2) ^ (y % 4 < 2))
					vertex[p].r = 0.0;
				else
					vertex[p].r = 1.0;

				vertex[p].g = (GLfloat) y / (GLfloat) GRIDH;
				vertex[p].b = 1.f - ((GLfloat) x / (GLfloat) GRIDW + (GLfloat) y / (GLfloat) GRIDH) / 2.f;

			}
        }
    }

    for (int i=0; i<VERTEXNUM;i++)
    {        
		quad[i] =i;
    }

	struct Vertex first;
	first.x = 0;
	first.y = 0;
	first.z = 0;
	first.r = 100;
	first.g = 100;
	first.b = 100;

	linked_list.vertex = &first;
	linked_list.previous = NULL;
	linked_list.next = NULL;

	struct Vertex second;
	struct Vertex third;

	second.x = 1;
	second.y = 1;
	second.z = 1;
	second.r = 200;
	second.g = 200;
	second.b = 200;

	third.x = 2;
	third.y = 2;
	third.z = 2;
	third.r = 220;
	third.g = 220;
	third.b = 220;

	char debugString[100] = { 0 };

	int count = NumberOfElements(linked_list);
	sprintf(debugString, "linked_list: %d elements \r\n", count);
	OutputDebugString(debugString);

	AddElement(&linked_list, &second);

	count = NumberOfElements(linked_list);
	sprintf(debugString, "linked_list: %d elements \r\n", count);
	OutputDebugString(debugString);

	AddElement(&linked_list, &third);

	count = NumberOfElements(linked_list);
	sprintf(debugString, "linked_list: %d elements \r\n", count);
	OutputDebugString(debugString);

	PrintElements(linked_list);

	update_vertex_array();
}

//========================================================================
// Draw scene
//========================================================================

void draw_scene(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // We don't want to modify the projection matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move back
    glTranslatef(0.0, 0.0, -zoom);
    // Rotate the view
    glRotatef(beta, 1.0, 0.0, 0.0);
    glRotatef(alpha, 0.0, 0.0, 1.0);

	int * indices;
	indices = malloc(vertex_array_length*sizeof(GLuint));
	for (int i = 0; i<vertex_array_length; i++)
	{
		indices[i] = i;
	}

    glDrawElements(GL_POINTS, vertex_array_length, GL_UNSIGNED_INT, indices);
	//glDrawElements(GL_POINTS, VERTEXNUM, GL_UNSIGNED_INT, quad);

    glfwSwapBuffers(window);
}


//========================================================================
// Initialize Miscellaneous OpenGL state
//========================================================================

void init_opengl(void)
{
    // Use Gouraud (smooth) shading
    glShadeModel(GL_SMOOTH);

    // Switch on the z-buffer
    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(struct Vertex), vertex);
    glColorPointer(3, GL_FLOAT, sizeof(struct Vertex), &vertex[0].r); // Pointer to the first color

    glPointSize(10.0);

    // Background color is black
    glClearColor(0, 0, 0, 0);
}


//========================================================================
// Print errors
//========================================================================

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


//========================================================================
// Handle key strokes
//========================================================================

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_SPACE:
            break;
        case GLFW_KEY_LEFT:
            alpha += 5;
            break;
        case GLFW_KEY_RIGHT:
            alpha -= 5;
            break;
        case GLFW_KEY_UP:
            beta -= 5;
            break;
        case GLFW_KEY_DOWN:
            beta += 5;
            break;
        case GLFW_KEY_PAGE_UP:
            zoom -= 0.25f;
            if (zoom < 0.f)
                zoom = 0.f;
            break;
        case GLFW_KEY_PAGE_DOWN:
            zoom += 0.25f;
            break;
        default:
            break;
    }
}


//========================================================================
// Callback function for mouse button events
//========================================================================

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &cursorX, &cursorY);
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


//========================================================================
// Callback function for cursor motion events
//========================================================================

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;

        cursorX = x;
        cursorY = y;
    }
}


//========================================================================
// Callback function for scroll events
//========================================================================

void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;
    mat4x4 projection;

    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    mat4x4_perspective(projection,
                       60.f * (float) M_PI / 180.f,
                       ratio,
                       1.f, 1024.f);
    glLoadMatrixf((const GLfloat*) projection);
}


//========================================================================
// main
//========================================================================

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    double t, dt_total, t_old;
    int width, height;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Wave Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    // Initialize OpenGL
    init_opengl();

    // Initialize simulation
    init_vertices();

    // Initialize timer
    t_old = glfwGetTime() - 0.01;

    while (!glfwWindowShouldClose(window))
    {
        draw_scene(window);
        glfwPollEvents();
    }

    exit(EXIT_SUCCESS);
}

