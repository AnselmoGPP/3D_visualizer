#pragma once

/*
*	This class allowes to visualize points in 2 ways. Coordinates are sent to the class as (x, y, z), where x goes to the front, y to the left and z upwards.
*		- send_data_as_vector(): Send a vector<vector<pnt3D>> where each main vector is a cluster of points. The visualizer will show each cluster with a different color.
*		- send_data_as_array(): Send an array of points and the visualizer will show all the points in black.
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

#include "shader.hpp"
#include "controls.hpp"

struct pnt3D{
	float x, y, z;
    pnt3D(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) { }
    pnt3D newData(float a, float b, float c) {
        x = a; y = b; z = c;
        return *this;
    }
};

#define BUFFER_SIZE 811*3*3

class visualizerClass {

	float points_buffer[BUFFER_SIZE];
	size_t arr_size = 0;				// Number of floats from points_buffer and colors_buffer that are going to be printed
	float colors_buffer[BUFFER_SIZE];
	float palette[21][3] = {
		   {1.0f,  0.0f,  0.0f},
		   {0.0f,  1.0f,  0.0f},
		   {0.0f,  0.0f,  1.0f},
		   {1.0f,  0.0f,  1.0f},
		   {1.0f,  1.0f,  0.0f},
		   {0.0f,  1.0f,  1.0f},
		   {0.5f,  0.0f,  0.0f},
		   {0.0f,  0.5f,  0.0f},
		   {0.0f,  0.0f,  0.5f},
		   {0.5f,  0.5f,  0.0f},
		   {0.5f,  0.0f,  0.5f},
		   {0.0f,  0.5f,  0.5f},
		   {0.75f,  0.25f,  0.25f},
		   {0.25f,  0.75f,  0.25f},
		   {0.25f,  0.25f,  0.75f},
		   {0.25f,  0.25f,  0.0f},
		   {0.25f,  0.0f,  0.25f},
		   {0.0f,  0.25f,  0.25f},
		   {0.75f,  0.75f,  0.0f},
		   {0.0f,  0.75f,  0.75f},
		   {0.75f, 0.0f,  0.75f}
	};

	std::mutex mut;
	
	GLFWwindow* window;			// The window object used to draw

	int run_thread();			// The thread where the visualizer is run

public:

	// Open a new thread that runs the visualizer
	void run() {
		std::thread running(&visualizerClass::run_thread, this);
		running.detach();
	}; 

    // Copies data from a vector to points_buffer and colors_buffer. No transformation to adapt to OGL coordinates system
	void send_data_as_vector(std::vector<std::vector<pnt3D>>& points){

		int color_index = 0;
		//std::cout << ">>>" << points.size() << std::endl;
		mut.lock();
		arr_size = 0;

		for (int i = 0; i < points.size(); i++) {
			//std::cout << points[i].size() << ", ";
			for (int j = 0; j < points[i].size(); j++) {				// X = -y;   Y = z ;   Z = -x
                points_buffer[arr_size] = points[i][j].x;
				colors_buffer[arr_size++] = palette[color_index][0];
                points_buffer[arr_size] = points[i][j].y;
				colors_buffer[arr_size++] = palette[color_index][1];
                points_buffer[arr_size] = points[i][j].z;
				colors_buffer[arr_size++] = palette[color_index][2];
			}
			if (color_index != 20) color_index++;
			else color_index = 0;
		}
		//std::cout << std::endl;
		mut.unlock();
	}

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
			colors_buffer[i * 3 + 0] = 0;
			colors_buffer[i * 3 + 1] = 0;
			colors_buffer[i * 3 + 2] = 0;
		}
		mut.unlock();
	}

    // Copies data from an array to points_buffer. Color is black. No transformation to adapt to OGL coordinates system
    void send_data_as_array(int number_points, float* arr) {		// This can be optimized

        mut.lock();
        arr_size = number_points * 3;

        for (int i = 0; i < number_points; i++) {
            points_buffer[i * 3 + 0] = arr[i * 3 + 0];	// x
            points_buffer[i * 3 + 1] = arr[i * 3 + 1];	// y
            points_buffer[i * 3 + 2] = arr[i * 3 + 2];	// z
            colors_buffer[i * 3 + 0] = 0;
            colors_buffer[i * 3 + 1] = 0;
            colors_buffer[i * 3 + 2] = 0;
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
};
