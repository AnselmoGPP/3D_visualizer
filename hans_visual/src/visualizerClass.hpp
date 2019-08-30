/*
*	TODO:
*		- Allow relative paths for calling LoadShaders()
*       - Dynamic allocation of data (vertex, lines, cubes)
*		- Modify size of points depending on distance
*		- Make certain macros (window width and height) really modifiable (controls.hpp does not contains them)
*		- Vextex in cubes are written at least 3 times. Optimize this
*		- GUI: Implement Stop (turn-off), Continue
*		- System for selecting a layer and adjusting transparency
*       - Cubes complete rotation (rotation method for a point)
*		- Be careful with static variables. They may make multiple windows problematic
*
*       > Parameters window (toolbar) (camera, transparency, axis...)
*       - Shaper with loop and without it
*		> Multilayer themes:
*			- Enter checkbox names
*       - Dynamic memory for palette and layer buffers
*       - HSI-RGB conversor for changing palette    https://es.wikipedia.org/wiki/Modelo_de_color_HSL
*       - Ordenar buffer para cubos para perfecta transparencia
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "shader.hpp"
#include "controls.hpp"

struct pnt3D {
	float x, y, z;

	pnt3D(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) { }

	pnt3D newData(float a, float b, float c) {
		x = a; y = b; z = c;
		return *this;
	}
};

struct cube3D {
	float X, Y, Z, width, height, length, rot_H, rot_V;      // rotation in radians

	// Parameters: x, y, z (cube's center), width, height, length, rot_H (horizontal rotation), rot_V (vertical rotation)
	cube3D(float x, float y, float z, float w, float h, float l, float rh, float rv) :
		X(x), Y(y), Z(z), width(w), height(h), length(l), rot_H(rh), rot_V(rv) { }
};

struct layer_system {

	layer_system(
		unsigned int point_lay = 0,		std::string *point_names = nullptr,		unsigned int *max_points = 0,
		unsigned int line_lay = 0,		std::string *line_names = nullptr,		unsigned int *max_lines = 0,
		unsigned int cubes_lay = 0,		std::string *cube_names = nullptr,		unsigned int *max_cubes = 0,
		unsigned int triangles_lay = 0,	std::string *triangle_names = nullptr,	unsigned int *max_triangles = 0
	) : 
		point_layers(point_lay), line_layers(line_lay), cube_layers(cubes_lay), triangle_layers(triangles_lay) { 

		point_layers_names = new std::string[point_layers];
		for (int i = 0; i < point_layers; i++) 
			point_layers_names[i] = point_names[i];

		line_layers_names = new std::string[line_layers];
		for (int i = 0; i < line_layers; i++)
			line_layers_names[i] = line_names[i];
	
		cube_layers_names = new std::string[cube_layers];
		for (int i = 0; i < cube_layers; i++)
			cube_layers_names[i] = cube_names[i];

		triangle_layers_names = new std::string[triangle_layers];
		for (int i = 0; i < triangle_layers; i++)
			triangle_layers_names[i] = triangle_names[i];
	}

	~layer_system() {

		delete [] point_layers_names;
		delete [] max_points;

		delete [] line_layers_names;
		delete [] max_lines;

		delete [] cube_layers_names;
		delete [] max_cubes;

		delete [] triangle_layers_names;
		delete [] max_triangles;
	}

	layer_system(const layer_system &obj) {

		point_layers = obj.point_layers;
		line_layers = obj.line_layers;
		cube_layers = obj.cube_layers;
		triangle_layers = obj.triangle_layers;


		point_layers_names = new std::string[point_layers];
		for (int i = 0; i < point_layers; i++)
			point_layers_names[i] = obj.point_layers_names[i];

		line_layers_names = new std::string[line_layers];
		for (int i = 0; i < line_layers; i++)
			line_layers_names[i] = obj.line_layers_names[i];

		cube_layers_names = new std::string[cube_layers];
		for (int i = 0; i < cube_layers; i++)
			cube_layers_names[i] = obj.cube_layers_names[i];

		triangle_layers_names = new std::string[triangle_layers];
		for (int i = 0; i < triangle_layers; i++)
			triangle_layers_names[i] = obj.triangle_layers_names[i];


		max_points = new unsigned int[point_layers];
		for (int i = 0; i < point_layers; i++)
			max_points[i] = obj.max_points[i];

		max_lines = new unsigned int[line_layers];
		for (int i = 0; i < line_layers; i++)
			max_lines[i] = obj.max_lines[i];

		max_cubes = new unsigned int[cube_layers];
		for (int i = 0; i < cube_layers; i++)
			max_cubes[i] = obj.max_cubes[i];

		max_triangles = new unsigned int[triangle_layers];
		for (int i = 0; i < triangle_layers; i++)
			max_triangles[i] = obj.max_triangles[i];
	}

	layer_system& operator=(const layer_system &obj) {

		point_layers = obj.point_layers;
		line_layers = obj.line_layers;
		cube_layers = obj.cube_layers;
		triangle_layers = obj.triangle_layers;


		point_layers_names = new std::string[point_layers];
		for (int i = 0; i < point_layers; i++)
			point_layers_names[i] = obj.point_layers_names[i];

		line_layers_names = new std::string[line_layers];
		for (int i = 0; i < line_layers; i++)
			line_layers_names[i] = obj.line_layers_names[i];

		cube_layers_names = new std::string[cube_layers];
		for (int i = 0; i < cube_layers; i++)
			cube_layers_names[i] = obj.cube_layers_names[i];

		triangle_layers_names = new std::string[triangle_layers];
		for (int i = 0; i < triangle_layers; i++)
			triangle_layers_names[i] = obj.triangle_layers_names[i];


		max_points = new unsigned int[point_layers];
		for (int i = 0; i < point_layers; i++)
			max_points[i] = obj.max_points[i];

		max_lines = new unsigned int[line_layers];
		for (int i = 0; i < line_layers; i++)
			max_lines[i] = obj.max_lines[i];

		max_cubes = new unsigned int[cube_layers];
		for (int i = 0; i < cube_layers; i++)
			max_cubes[i] = obj.max_cubes[i];

		max_triangles = new unsigned int[triangle_layers];
		for (int i = 0; i < triangle_layers; i++)
			max_triangles[i] = obj.max_triangles[i];
	}

	unsigned int point_layers;
	std::string *point_layers_names;
	unsigned int *max_points;

	unsigned int line_layers;
	std::string *line_layers_names;
	unsigned int *max_lines;

	unsigned int cube_layers;
	std::string *cube_layers_names;
	unsigned int *max_cubes;

	unsigned int triangle_layers;
	std::string *triangle_layers_names;
	unsigned int *max_triangles;
};

// Define some maximum number of elements:
#define NUM_OF_VERTEX   2500
#define NUM_OF_LINES    500
#define NUM_OF_CUBES    100

// Define the kind of the additional array you send
enum data_buffer{
    colors,             // Define colors of each point
    categories,         // Define category of each point. If number of categories > colors in the palette, the visualizer starts again from the beginning of the pallete
    gradient            // Define a gradient for each point that goes from a minimum to a maximum value. You enter the absolute minimum and maximum.
};

enum object_type{ points, lines, cubes };

class visualizerClass {

	controls &cam{ camera };

	std::mutex mut_points, mut_cubes, mut_lines;

	GLFWwindow* window;			// The window object used to draw

	int run_thread();			// The thread where the visualizer is run

    // GUI state
    bool existing_checkboxes[3][5]	= { 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0 };
    bool checkboxes_values[3][5]	= { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 };
	bool show_checkboxes = 1;
	bool show_data = 0;
	bool show_options = 0;
    bool cam_sphere = 1;			// 1: Sphere,  0: FPS
	bool cam_FPS = 0;
	std::string data_window[10];
	std::mutex mut_fill_data;
	float backg_color[3] = { 0.5, 0.5, 0.5 };

	// Graphical User Interface rendering
	void create_windows();
	void create_demo_windows();
	void change_alpha_channel(int object);			// Change the alpha values from the corresponding color buffer: 1(points), 2(lines), 3(boxes)

	// Points data
	float points_buffer[NUM_OF_VERTEX][3];          // Stores all the coordinates of all the points
	size_t points_to_print = 0;                     // Number of points that are going to be printed
	float points_colors_buffer[NUM_OF_VERTEX][4];	// Stores the RGBA colors for each point
    int points_palette_size = 21;
    float (*points_palette)[3];

	// Lines data
	float lines_buffer[NUM_OF_LINES][2][3];
	size_t lines_to_print = 0;
	float lines_colors_buffer[NUM_OF_LINES][2][4];
    int lines_palette_size = 21;
    float (*lines_palette)[3];

	// Cubes data
	float cubes_buffer[NUM_OF_CUBES][12 * 3][3];
	size_t cubes_to_print = 0;
	float cubes_colors_buffer[NUM_OF_CUBES][12 * 3][4];
    int cubes_palette_size = 21;
    float (*cubes_palette)[3];

	float points_alpha_channel = 1.0f;
	float lines_alpha_channel = 1.0f;
	float cubes_alpha_channel = 0.3f;

	// Parameters: X, Y (cube's center), x, y (point), rot (radians). It considers x as OpenGL's x, and y as OpenGL's -z.
    void rotation_H(float &x, float &y, float X, float Y, float rot);

    // In-main-loop functions for loading data to the GPU
    void load_points(GLuint vertexbuffer, GLuint colorbuffer);
    void load_lines(GLuint linesbuffer, GLuint linescolors);
    void load_cubes(GLuint cubesbuffer, GLuint cubes_colors_buffer);

public:
     visualizerClass();
    ~visualizerClass();

    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

	// Send points to print. Optionally, include an additional array with the category of each point (cluster) or the color of each point, and specify what type of array it is:
    // - 'categories': Should be numbers starting from 0 to the number of categories - 1. When don't specified, the additional array is considered 'categories'.
    // - 'gradient': Must include the minimum and maximum values (inclusive). If not specified, the minimum is 0 and the maximum is 1.
    // - 'colors'
	// For more info, look at the macros definition (visualizerClass.hpp).
	// When no category or color array is sent, points are black.
    void send_points(int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

	// Send an array containing the points coordinates you want to bind with lines, and the number of points you want to use, including the gap-points with coordinates (1.2, 3.4, 5.6) (include this number in the number of points).
    void send_lines(int number_lines, const float *points, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

	void send_triangles(int number_triangles, const float *points, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of cubes (cube3D) to print them
    void send_cubes(int number_cubes, const cube3D *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Bonus methods --------------------------------

    // Send a new palette of colors to replace the current one
    void send_palette(float *gradient_palette, int number_colors, object_type obj);

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings);

    // This represents the function [ y(x) = a + bx + cx^2 + dx^3 ]. It outputs the y value for the inclusive range [xmin, xmax] (including extremes). The sample variable is the number of segments (the result_array will store 'sample' + 1 elements)
    void pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d);

	// Pass a pointer to an array[12][3] to store the icosahedron vertices. You must provide the radius too.
	void icosahedron(float side_length, float(*points)[3]);

    // Obsolete -------------------------------------

    // Copies data from a vector to points_buffer and colors_buffer. No transformation to adapt to OGL coordinates system
    void send_data_as_vector(std::vector<std::vector<pnt3D>>& vecPoints);

    // Copies data from an array to points_buffer. Color is black. No transformation to adapt to OGL coordinates system
    void send_data_as_array(int number_points, const float* arr);       // This can be optimized
};
