# Visualizer class

This is a library for 3D visualization using OpenGL. It contains a class called "visualizerClass" that takes care of everything you need for drawing point clouds. A basic point class (x, y, z) called "pnt3D" is included. This library compiles in Ubuntu (make) and Windows (MVS).

It includes CMakeLists ready for building the visualizer for Ubuntu and Windows.

External libraries used:

- GLEW 2.1.0
- GLFW 3.3
- GLM 0.9.9.5

Things to take into account:

- Folder hans_visual/CMakeLists includes CMakeLists templates for building your applications using this library in Ubuntu (make) or Windows (MVS). Differences in the CMakeLists involve different directories for the library files (.a, .lib) and a linking of additional libraries in Ubuntu. 

- Write the correct directory for the shader files in visualizerClass.cpp

- You can specify in your app's CMakeLists which files to use for Debug or Release modes (you have to had compiled the visualizer in Debug or Release). Then, use MVS or your Ubuntu compiler to compile in Debug or Release. Also, use your compiler to compile for a certain architecture (x86, x64).

- More: https://sciencesoftcode.wordpress.com/2019/04/04/building-on-ubuntu/


Code examples:

	#include "visualizerClass.hpp"

	visualizerClass visual;
	visual.run();						// Open a window
	visual.send_data_as_array(100, &(data[0][0]));		// Send a pointer to and array[][3] and the number of points to include (black points)
	visual.send_data_as_vector(cluster_set);		// Send a vector<vector<pnt3D>> to get a point cloud with clusters (colored points)
