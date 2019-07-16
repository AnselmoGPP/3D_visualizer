# Visualizer class

This is a library for 3D visualization using OpenGL. It contains a class called "visualizerClass" that takes care of everything you need for drawing point clouds. A basic point class (x, y, z) called "pnt3D" is included.

It includes CMakeLists ready for building the visualizer for Ubuntu and Windows.

External libraries used:

- GLEW 2.1.0
- GLFW 3.3
- GLM 0.9.9.5

Code examples:

	#include "visualizerClass.hpp"

	visualizerClass visual;
	visual.run();						// Open a window
	visual.send_data_as_array(100, &(data[0][0]));		// Send a pointer to and array[][3] and the number of points to include (black points)
	visual.send_data_as_vector(cluster_set);		// Send a vector<vector<pnt3D>> to get a point cloud with clusters (colored points)
