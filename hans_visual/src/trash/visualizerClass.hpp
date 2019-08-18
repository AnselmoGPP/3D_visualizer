#pragma once

/*
*	This class allowes to visualize points in 2 ways. Coordinates are sent to the class as (x, y, z), where x goes to the front, y to the left and z upwards.
*		- send_data_as_vector(): Send a vector<vector<pnt3D>> where each main vector is a cluster of points. The visualizer will show each cluster with a different color.
*		- send_data_as_array(): Send an array of points and the visualizer will show all the points in black.
*
*	Configuration:
*		- NUM_OF_VERTEX
*		- NUM_OF_LINES
*
*	TODO:
*		- Allow relative paths for calling LoadShaders()
*		- Modify size of points depending on distance
*		- Make certain macros (window width and height) really modifiable (controls.hpp does not contains them)
*		- Vextex in cubes are written at least 3 times. Optimize this
*		> GUI: Implement Stop (turn-off), Continue, show/quit boxes, show/quit points...
*		- Implement transparency option
*       - Dynamic allocation of data (vertex, lines, cubes)
*       > Spherical Camera
*		- Allow to draw disconnected lines
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//#include "imgui.h"
//#include "examples/imgui_impl_glfw.h"
//#include "examples/imgui_impl_opengl3.h"

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

    // Parameters: x, y, z (cube's center), width, length, height, rot_H (horizontal rotation), rot_V (vertical rotation)
    cube3D(float x, float y, float z, float w, float h, float l, float rh, float rv) :
        X(x), Y(y), Z(z), width(w), height(h), length(l), rot_H(rh), rot_V(rv) { }
};

// Define some maximum number of elements:
#define NUM_OF_VERTEX 2500
#define NUM_OF_LINES 500
#define NUM_OF_CUBES 100
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Choose only one camera system from the following:
#define FPS 1
#define SPHERE 0

class visualizerClass {

    // Points data
    float points_buffer[NUM_OF_VERTEX][3];          // Stores all the coordinates of all the points
    size_t points_to_print = 0;                     // Number of points that are going to be printed
    float points_colors_buffer[NUM_OF_VERTEX][4];	// Stores the RGBA colors for each point
    float palette[21][3] = {
           {1.0f,	0.0f,	0.0f},
           {0.0f,	1.0f,	0.0f},
           {0.0f,	0.0f,	1.0f},
           {1.0f,	0.0f,	1.0f},
           {1.0f,	1.0f,	0.0f},
           {0.0f,	1.0f,	1.0f},
           {0.5f,	0.0f,	0.0f},
           {0.0f,	0.5f,	0.0f},
           {0.0f,	0.0f,	0.5f},
           {0.5f,	0.5f,	0.0f},
           {0.5f,	0.0f,	0.5f},
           {0.0f,	0.5f,	0.5f},
           {0.75f,	0.25f,	0.25f},
           {0.25f,	0.75f,	0.25f},
           {0.25f,	0.25f,	0.75f},
           {0.25f,	0.25f,	0.0f},
           {0.25f,	0.0f,	0.25f},
           {0.0f,	0.25f,	0.25f},
           {0.75f,	0.75f,	0.0f},
           {0.0f,	0.75f,	0.75f},
           {0.75f,	0.0f,	0.75f}
    };

    // Lines data
	float lines_buffer[NUM_OF_LINES][2][3];
	size_t lines_to_print = 0;
	float lines_colors_buffer[NUM_OF_LINES][2][4];

    // Cubes data
    float cubes_buffer[NUM_OF_CUBES][12*3][3];
    size_t cubes_to_print = 0;
    float cubes_colors_buffer[NUM_OF_CUBES][12*3][4];

	std::mutex mut_points, mut_cubes, mut_lines;
	
	GLFWwindow* window;			// The window object used to draw

	int run_thread();			// The thread where the visualizer is run

    // Parameters: X, Y (cube's center), x, y (point), rot (radians). It considers x as OpenGL's x, and y as OpenGL's -z.
    void rotation_H(float &x, float &y, float X, float Y, float rot) {

        y = -y;

        float alpha = atan(y / x);
        if      (x < 0 && y >= 0) alpha += 3.1415926535f;
        else if (y < 0 && x <  0) alpha += 3.1415926535f;
        else if (y < 0 && x >= 0) alpha += 2.f * 3.1415926535f;

        float hip = sqrt(x * x + y * y);

        float beta = alpha + rot;

        x = hip * cos(beta) + X;
        y = - (hip * sin(beta)) + Y;
    }

public:
    float points_alpha_channel = 1.0;
    float cubes_alpha_channel  = 0.3;
	float lines_alpha_channel  = 1.0;

	// Create a window and open a new thread that runs the visualizer
	int run() {

		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);								// x4 antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// GLFW version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



		std::thread running(&visualizerClass::run_thread, this);
		running.detach();
		return 0;
	}; 

    // Copies data from a vector to points_buffer and colors_buffer. No transformation to adapt to OGL coordinates system
    void send_data_as_vector(std::vector<std::vector<pnt3D>>& vecPoints){

		int color_index = 0;

		mut_points.lock();
        points_to_print = 0;

        for (int i = 0; i < vecPoints.size(); i++) {
            for (int j = 0; j < vecPoints[i].size(); j++)
            {
                points_buffer[points_to_print][0] = vecPoints[i][j].x;
                points_buffer[points_to_print][1] = vecPoints[i][j].y;
                points_buffer[points_to_print][2] = vecPoints[i][j].z;

                points_colors_buffer[points_to_print][0] = palette[color_index][0];
                points_colors_buffer[points_to_print][1] = palette[color_index][1];
                points_colors_buffer[points_to_print][2] = palette[color_index][2];
                points_colors_buffer[points_to_print][3] = points_alpha_channel;

                points_to_print++;

                if(points_to_print == NUM_OF_VERTEX) {
                    std::cout << "Too many points for the points-buffer (" << NUM_OF_VERTEX << " max.)" << std::endl;
                    return;
                }
			}
			if (color_index != 20) color_index++;
			else color_index = 0;
		}
		mut_points.unlock();
	}

	// Copies data from an array to points_buffer. Color is black. No transformation to adapt to OGL coordinates system
    void send_data_as_array(int number_points, const float* arr) {		// This can be optimized

        if(number_points > NUM_OF_VERTEX) {
            std::cout << "Too many points for the points-buffer (" << number_points << " > " << NUM_OF_VERTEX << ')' << std::endl;
            number_points = NUM_OF_VERTEX;
        }
		else points_to_print = number_points;

		mut_points.lock();

        for (int i = 0; i < number_points; i++)
        {
            points_buffer[i][0] = arr[i * 3 + 0];
            points_buffer[i][1] = arr[i * 3 + 1];
            points_buffer[i][2] = arr[i * 3 + 2];

            points_colors_buffer[i][0] = 0;
            points_colors_buffer[i][1] = 0;
            points_colors_buffer[i][2] = 0;
            points_colors_buffer[i][3] = points_alpha_channel;
		}
		mut_points.unlock();
	}

    // Send array of cubes (cube3D) to print them
    void send_cubes(size_t number_cubes, const cube3D * arr){

        if(number_cubes > NUM_OF_CUBES) {
            std::cout << "Too many cubes for the cubes-buffer (" << number_cubes << " > " << NUM_OF_CUBES << ')' << std::endl;
            cubes_to_print = NUM_OF_CUBES;
        }
        else cubes_to_print = number_cubes;

        float X, Y, Z, x, y, z, rot_H, rot_V;

		mut_cubes.lock();

        // Fill cubes_buffer                            >>> This draws triangles anti-clockwise
        for(size_t i = 0; i < cubes_to_print; i++){

            X       = arr[i].X;
            Y       = arr[i].Y;
            Z       = arr[i].Z;
            x       = arr[i].width  / 2;
            y       = arr[i].height / 2;
            z       = arr[i].length / 2;
            rot_H   = arr[i].rot_H;
            rot_V   = arr[i].rot_V;
/*
            // Computations used with no rotations
            float x1 = X - x,   y1 = Y + y,   z1 = Z + z;
            float x2 = X - x,   y2 = Y - y,   z2 = Z + z;
            float x3 = X + x,   y3 = Y + y,   z3 = Z + z;
            float x4 = X + x,   y4 = Y - y,   z4 = Z + z;
            float x5 = X + x,   y5 = Y + y,   z5 = Z - z;
            float x6 = X + x,   y6 = Y - y,   z6 = Z - z;
            float x7 = X - x,   y7 = Y + y,   z7 = Z - z;
            float x8 = X - x,   y8 = Y - y,   z8 = Z - z;
*/
            float x1 = - x,   y1 = Y + y,   z1 = + z;
            float x2 = - x,   y2 = Y - y,   z2 = + z;
            float x3 = + x,   y3 = Y + y,   z3 = + z;
            float x4 = + x,   y4 = Y - y,   z4 = + z;
            float x5 = + x,   y5 = Y + y,   z5 = - z;
            float x6 = + x,   y6 = Y - y,   z6 = - z;
            float x7 = - x,   y7 = Y + y,   z7 = - z;
            float x8 = - x,   y8 = Y - y,   z8 = - z;

            rotation_H(x1, z1, X, Z, rot_H);
            rotation_H(x2, z2, X, Z, rot_H);
            rotation_H(x3, z3, X, Z, rot_H);
            rotation_H(x4, z4, X, Z, rot_H);
            rotation_H(x5, z5, X, Z, rot_H);
            rotation_H(x6, z6, X, Z, rot_H);
            rotation_H(x7, z7, X, Z, rot_H);
            rotation_H(x8, z8, X, Z, rot_H);

            // 1-2-3
            cubes_buffer[i][0][0] = x1;
            cubes_buffer[i][0][1] = y1;
            cubes_buffer[i][0][2] = z1;
            cubes_buffer[i][1][0] = x2;
            cubes_buffer[i][1][1] = y2;
            cubes_buffer[i][1][2] = z2;
            cubes_buffer[i][2][0] = x3;
            cubes_buffer[i][2][1] = y3;
            cubes_buffer[i][2][2] = z3;
            // 2-4-3
            cubes_buffer[i][3][0] = x2;
            cubes_buffer[i][3][1] = y2;
            cubes_buffer[i][3][2] = z2;
            cubes_buffer[i][4][0] = x4;
            cubes_buffer[i][4][1] = y4;
            cubes_buffer[i][4][2] = z4;
            cubes_buffer[i][5][0] = x3;
            cubes_buffer[i][5][1] = y3;
            cubes_buffer[i][5][2] = z3;
            // 3-4-5
            cubes_buffer[i][6][0] = x3;
            cubes_buffer[i][6][1] = y3;
            cubes_buffer[i][6][2] = z3;
            cubes_buffer[i][7][0] = x4;
            cubes_buffer[i][7][1] = y4;
            cubes_buffer[i][7][2] = z4;
            cubes_buffer[i][8][0] = x5;
            cubes_buffer[i][8][1] = y5;
            cubes_buffer[i][8][2] = z5;
            // 4-6-5
            cubes_buffer[i][9][0]  = x4;
            cubes_buffer[i][9][1]  = y4;
            cubes_buffer[i][9][2]  = z4;
            cubes_buffer[i][10][0] = x6;
            cubes_buffer[i][10][1] = y6;
            cubes_buffer[i][10][2] = z6;
            cubes_buffer[i][11][0] = x5;
            cubes_buffer[i][11][1] = y5;
            cubes_buffer[i][11][2] = z5;
            // 5-6-7
            cubes_buffer[i][12][0] = x5;
            cubes_buffer[i][12][1] = y5;
            cubes_buffer[i][12][2] = z5;
            cubes_buffer[i][13][0] = x6;
            cubes_buffer[i][13][1] = y6;
            cubes_buffer[i][13][2] = z6;
            cubes_buffer[i][14][0] = x7;
            cubes_buffer[i][14][1] = y7;
            cubes_buffer[i][14][2] = z7;
            // 6-8-7
            cubes_buffer[i][15][0] = x6;
            cubes_buffer[i][15][1] = y6;
            cubes_buffer[i][15][2] = z6;
            cubes_buffer[i][16][0] = x8;
            cubes_buffer[i][16][1] = y8;
            cubes_buffer[i][16][2] = z8;
            cubes_buffer[i][17][0] = x7;
            cubes_buffer[i][17][1] = y7;
            cubes_buffer[i][17][2] = z7;
            // 7-8-1
            cubes_buffer[i][18][0] = x7;
            cubes_buffer[i][18][1] = y7;
            cubes_buffer[i][18][2] = z7;
            cubes_buffer[i][19][0] = x8;
            cubes_buffer[i][19][1] = y8;
            cubes_buffer[i][19][2] = z8;
            cubes_buffer[i][20][0] = x1;
            cubes_buffer[i][20][1] = y1;
            cubes_buffer[i][20][2] = z1;
            // 8-2-1
            cubes_buffer[i][21][0] = x8;
            cubes_buffer[i][21][1] = y8;
            cubes_buffer[i][21][2] = z8;
            cubes_buffer[i][22][0] = x2;
            cubes_buffer[i][22][1] = y2;
            cubes_buffer[i][22][2] = z2;
            cubes_buffer[i][23][0] = x1;
            cubes_buffer[i][23][1] = y1;
            cubes_buffer[i][23][2] = z1;
            // 1-3-7
            cubes_buffer[i][24][0] = x1;
            cubes_buffer[i][24][1] = y1;
            cubes_buffer[i][24][2] = z1;
            cubes_buffer[i][25][0] = x3;
            cubes_buffer[i][25][1] = y3;
            cubes_buffer[i][25][2] = z3;
            cubes_buffer[i][26][0] = x7;
            cubes_buffer[i][26][1] = y7;
            cubes_buffer[i][26][2] = z7;
            // 3-5-7
            cubes_buffer[i][27][0] = x3;
            cubes_buffer[i][27][1] = y3;
            cubes_buffer[i][27][2] = z3;
            cubes_buffer[i][28][0] = x5;
            cubes_buffer[i][28][1] = y5;
            cubes_buffer[i][28][2] = z5;
            cubes_buffer[i][29][0] = x7;
            cubes_buffer[i][29][1] = y7;
            cubes_buffer[i][29][2] = z7;
            // 2-8-4
            cubes_buffer[i][30][0] = x2;
            cubes_buffer[i][30][1] = y2;
            cubes_buffer[i][30][2] = z2;
            cubes_buffer[i][31][0] = x8;
            cubes_buffer[i][31][1] = y8;
            cubes_buffer[i][31][2] = z8;
            cubes_buffer[i][32][0] = x4;
            cubes_buffer[i][32][1] = y4;
            cubes_buffer[i][32][2] = z4;
            // 4-8-6
            cubes_buffer[i][33][0] = x4;
            cubes_buffer[i][33][1] = y4;
            cubes_buffer[i][33][2] = z4;
            cubes_buffer[i][34][0] = x8;
            cubes_buffer[i][34][1] = y8;
            cubes_buffer[i][34][2] = z8;
            cubes_buffer[i][35][0] = x6;
            cubes_buffer[i][35][1] = y6;
            cubes_buffer[i][35][2] = z6;
        }

        // Fill cubes_color_buffer
        for(int j = 0; j < NUM_OF_CUBES; j++)
            for(int k = 0; k < 12*3; k++)
            {
                cubes_colors_buffer[j][k][0] = 0.8f;
                cubes_colors_buffer[j][k][1] = 0.1f;
                cubes_colors_buffer[j][k][2] = 0.6f;
                cubes_colors_buffer[j][k][3] = cubes_alpha_channel;
            }

		mut_cubes.unlock();
    }

	// Send an array containing the points coordinates you want to bind with lines, and the number of points you want to use, including the gap-points with coordinates (0, 0, 0) (include this number in the number of points).
	void send_lines(size_t number_points, const float *points) {

		lines_to_print = 0;

		mut_lines.lock();
		for (int i = 0; i < number_points -1; i++)
		{
			if (lines_to_print == NUM_OF_LINES) {
				std::cout << "Too many lines for the lines-buffer (" << NUM_OF_LINES << "max.)" << std::endl;
				break;
			}

			//if (points[i * 6 + 3] == 0 && points[i * 6 + 4] == 0 && points[i * 6 + 5] == 0) {
			//	i++;
			//	continue;
			//}

			lines_buffer[i][0][0] = points[i * 3 + 0];
			lines_buffer[i][0][1] = points[i * 3 + 1];
			lines_buffer[i][0][2] = points[i * 3 + 2];
			lines_buffer[i][1][0] = points[i * 3 + 3];
			lines_buffer[i][1][1] = points[i * 3 + 4];
			lines_buffer[i][1][2] = points[i * 3 + 5];

			//std::cout << i << ' ' << lines_buffer[i][0][0] << ' ' << lines_buffer[i][0][1] << ' ' << lines_buffer[i][0][2] << std::endl;
			//std::cout << i << ' ' << lines_buffer[i][1][0] << ' ' << lines_buffer[i][1][1] << ' ' << lines_buffer[i][1][2] << std::endl;

			++lines_to_print; 

			lines_colors_buffer[i][0][0] = 0;
			lines_colors_buffer[i][0][1] = 0;
			lines_colors_buffer[i][0][2] = 0;
			lines_colors_buffer[i][0][3] = lines_alpha_channel;
			lines_colors_buffer[i][1][0] = 0;
			lines_colors_buffer[i][1][1] = 0;
			lines_colors_buffer[i][1][2] = 0;
			lines_colors_buffer[i][1][3] = lines_alpha_channel;
		}
		mut_lines.unlock();
	}

	// --------------------- Too specific members --------------------------------------------------------------------------
/*
    // Copies data from a vector to points_buffer and colors_buffer.
    void send_data_as_vector2(std::vector<std::vector<pnt3D>>& points){

        int color_index = 0;
        //std::cout << ">>>" << points.size() << std::endl;
        mut.lock();
        arr_size = 0;

        for (int i = 0; i < points.size(); i++) {
            //std::cout << points[i].size() << ", ";
            for (int j = 0; j < points[i].size(); j++) {				// X = -y;   Y = z ;   Z = -x
                points_buffer[arr_size] = -points[i][j].y;
                colors_buffer[arr_size++] = palette[color_index][0];
                points_buffer[arr_size] =  points[i][j].z;
                colors_buffer[arr_size++] = palette[color_index][1];
                points_buffer[arr_size] = -points[i][j].x;
                colors_buffer[arr_size++] = palette[color_index][2];
				colors_buffer[arr_size++] = points_alpha_channel;
            }
            if (color_index != 20) color_index++;
            else color_index = 0;
        }
        //std::cout << std::endl;
        mut.unlock();
    }

	// Copies data from an array to points_buffer. Color is black.
    void send_data_as_array2(int number_points, float* arr) {		// This can be optimized

		mut.lock();
		arr_size = number_points * 3;

		for (int i = 0; i < number_points; i++) {
			points_buffer[i * 3 + 0] = -arr[i * 3 + 1];	// y
			points_buffer[i * 3 + 1] =  arr[i * 3 + 2];	// z
			points_buffer[i * 3 + 2] = -arr[i * 3 + 0];	// x
            colors_buffer[i * 4 + 0] = 0;
            colors_buffer[i * 4 + 1] = 0;
            colors_buffer[i * 4 + 2] = 0;
            colors_buffer[i * 4 + 3] = points_alpha_channel;
		}
		mut.unlock();
	}

	// Prints a certain number of values inside points_buffer and colors_buffer
	void debug_print_buffers(int number_values) {

		std::cout << "Points: ";
		for (int i = 0; i < number_values; i++) {
			std::cout << points_buffer[i] << ", ";
		}
		std::cout << std::endl;
		std::cout << "Colors: ";
		for (int i = 0; i < number_values; i++) {
			std::cout << colors_buffer[i] << ", ";
		}
		std::cout << std::endl << "--------" << std::endl;
	}
    */

};