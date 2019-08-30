// http://csharphelper.com/blog/2015/12/build-a-geodesic-sphere-with-wpf-and-c/

#include "visualizerClass.hpp"
#include <cmath>
#include <chrono>

int main() {

	visualizerClass visual;
    visual.open_window();                                   // Open a visualizer window

	// Draw points ------------------------------
    float pnts[12][3];
    visual.icosahedron(2, &pnts[0]);						// Fill the icos[] array with points coordinates (in this case 12 points, with 3 coordinates each)
    float points_categories[12] = { 0,1,2,3,4,5,6,7,8,10,10,10 };
    float points_colors[12][3] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
    float points_gradient_palette[21][3] = {
        0.0f, 0.0f, 0.00f,
        0.0f, 0.0f, 0.05f,
        0.0f, 0.0f, 0.10f,
        0.0f, 0.0f, 0.15f,
        0.0f, 0.0f, 0.20f,
        0.0f, 0.0f, 0.25f,
        0.0f, 0.0f, 0.30f,
        0.0f, 0.0f, 0.35f,
        0.0f, 0.0f, 0.40f,
        0.0f, 0.0f, 0.45f,
        0.0f, 0.0f, 0.50f,
        0.0f, 0.0f, 0.55f,
        0.0f, 0.0f, 0.60f,
        0.0f, 0.0f, 0.65f,
        0.0f, 0.0f, 0.70f,
        0.0f, 0.0f, 0.75f,
        0.0f, 0.0f, 0.80f,
        0.0f, 0.0f, 0.85f,
        0.0f, 0.0f, 0.90f,
        0.0f, 0.0f, 0.95f,
        0.0f, 0.0f, 1.00f
    };
	float points_gradients[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 };

    // >>> Different ways of drawing points
    //visual.send_data_as_array(12, &icos[0][0]);       // Load the points on the visualizer. You can also load them from a vector<vector<pnt3D>> using:   visual.send_data_as_vector(cluster_set);
    visual.send_points(12, &pnts[0][0]);
    visual.send_points(12, &pnts[0][0], points_categories, categories);		// Possible flags: CATEGORIES, COLORS, GRADIENT.
    visual.send_points(12, &pnts[0][0], points_categories);					// By default, interpreted as CATEGORIES

    visual.send_points(12, &pnts[0][0], &points_colors[0][0], colors);

    visual.send_palette(&points_gradient_palette[0][0], 21, points);
    visual.send_points(12, &pnts[0][0], points_gradients, gradient, 1, 12);
    visual.send_points(12, &pnts[0][0], points_gradients, gradient, 6, 7);

	// Draw cubes -------------------------------
	cube3D myCubes[3] = {
			cube3D(0, 0, 0, 1, 1, 1, 0.0*3.1415, 0),
			cube3D(1, 1, 1, 2, 1, 0.5, 0.3*3.1415, 0),
			cube3D(1, -1, 0, 1, 0.5, 0.1, 0.6*3.1415, 0)
	};
    float cubes_categories[3] = {0, 1, 2};
    float cubes_colors[3][3] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };
    float cubes_gradient_palette[21][3] = {
        0.0f, 0.0f, 0.00f,
        0.0f, 0.0f, 0.05f,
        0.0f, 0.0f, 0.10f,
        0.0f, 0.0f, 0.15f,
        0.0f, 0.0f, 0.20f,
        0.0f, 0.0f, 0.25f,
        0.0f, 0.0f, 0.30f,
        0.0f, 0.0f, 0.35f,
        0.0f, 0.0f, 0.40f,
        0.0f, 0.0f, 0.45f,
        0.0f, 0.0f, 0.50f,
        0.0f, 0.0f, 0.55f,
        0.0f, 0.0f, 0.60f,
        0.0f, 0.0f, 0.65f,
        0.0f, 0.0f, 0.70f,
        0.0f, 0.0f, 0.75f,
        0.0f, 0.0f, 0.80f,
        0.0f, 0.0f, 0.85f,
        0.0f, 0.0f, 0.90f,
        0.0f, 0.0f, 0.95f,
        0.0f, 0.0f, 1.00f
    };
	float cubes_gradients[3] = { 1,6,12 };

    // >>> Different ways of drawing cubes
    visual.send_cubes(2, myCubes);                                          // Load boxes on the visualizer

    visual.send_cubes(3, myCubes, cubes_categories, categories);            // Possible flags: CATEGORIES, COLORS, GRADIENT.
    visual.send_cubes(3, myCubes, cubes_categories);                        // By default, interpreted as CATEGORIES

    visual.send_cubes(3, myCubes, &cubes_colors[0][0], colors);

    visual.send_palette(&cubes_gradient_palette[0][0], 21, cubes);
    visual.send_cubes(3, myCubes, cubes_gradients, gradient, 1, 12);
    visual.send_cubes(3, myCubes, cubes_gradients, gradient, 0, 0);

	// Draw lines -------------------------------
    float box[9][3] = {
		-3,  3,  3,
		-3, -3,  3,
		 3, -3,  3,
		 3,  3,  3,
		 3,  3, -3,
		 3, -3, -3,
		-3, -3, -3,
		-3,  3, -3,
        -3,  3,  3 };
	//visual.send_lines(9, &join_points[0][0]);					// Load points that will be used to draw lines

	float parable[101][3];
    visual.pol_3th_degree(&parable[0][0], -10, 10, 100, 0, 0, 1, 0);	// Fill another array
	//visual.send_lines(101, &pol_3th[0][0]);					// Erase the previous lines-buffer and draws a new one

    float myLines[101 + 1 + 9][3];							// Fill this array with the 2 lines we created previously + one line jump between them
	int i;
	for (i = 0; i < 101; i++) {
        myLines[i][0] = parable[i][0];
        myLines[i][1] = parable[i][1];
        myLines[i][2] = parable[i][2];
	}
    myLines[i][0] = 1.2f;				// This points signals a jump between lines
    myLines[i][1] = 3.4f;
    myLines[i][2] = 5.6f;
	i++;
	for (int j = 0; j < 9; j++) {
        myLines[i + j][0] = box[j][0];
        myLines[i + j][1] = box[j][1];
        myLines[i + j][2] = box[j][2];
	}

    float lines_categories[101 + 1 + 9 - 1] = {     // Send a category for each segment (number of segments = number of points - 1)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 5,       // << These two segments are the line jump. They are not going to be displayed on screen
        1, 1, 1, 1, 2, 2, 2, 2
    };

    float lines_colors[101 + 1 + 9 - 1][3] = {      // Send a color for each segment (number of segments = number of points - 1)
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,

        0.0f, 0.0f, 0.0,          // << This is the line jump
        0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

	float lines_gradient_palette[101][3] = {
		0.00f, 0.0f, 0.0f,
		0.01f, 0.0f, 0.0f,
		0.02f, 0.0f, 0.0f,
		0.03f, 0.0f, 0.0f,
		0.04f, 0.0f, 0.0f,
		0.05f, 0.0f, 0.0f,
		0.06f, 0.0f, 0.0f,
		0.07f, 0.0f, 0.0f,
		0.08f, 0.0f, 0.0f,
		0.09f, 0.0f, 0.0f,

		0.10f, 0.0f, 0.0f,
		0.11f, 0.0f, 0.0f,
		0.12f, 0.0f, 0.0f,
		0.13f, 0.0f, 0.0f,
		0.14f, 0.0f, 0.0f,
		0.15f, 0.0f, 0.0f,
		0.16f, 0.0f, 0.0f,
		0.17f, 0.0f, 0.0f,
		0.18f, 0.0f, 0.0f,
		0.19f, 0.0f, 0.0f,

		0.20f, 0.0f, 0.0f,
		0.21f, 0.0f, 0.0f,
		0.22f, 0.0f, 0.0f,
		0.23f, 0.0f, 0.0f,
		0.24f, 0.0f, 0.0f,
		0.25f, 0.0f, 0.0f,
		0.26f, 0.0f, 0.0f,
		0.27f, 0.0f, 0.0f,
		0.28f, 0.0f, 0.0f,
		0.29f, 0.0f, 0.0f,

		0.30f, 0.0f, 0.0f,
		0.31f, 0.0f, 0.0f,
		0.32f, 0.0f, 0.0f,
		0.33f, 0.0f, 0.0f,
		0.34f, 0.0f, 0.0f,
		0.35f, 0.0f, 0.0f,
		0.36f, 0.0f, 0.0f,
		0.37f, 0.0f, 0.0f,
		0.38f, 0.0f, 0.0f,
		0.39f, 0.0f, 0.0f,

		0.40f, 0.0f, 0.0f,
		0.41f, 0.0f, 0.0f,
		0.42f, 0.0f, 0.0f,
		0.43f, 0.0f, 0.0f,
		0.44f, 0.0f, 0.0f,
		0.45f, 0.0f, 0.0f,
		0.46f, 0.0f, 0.0f,
		0.47f, 0.0f, 0.0f,
		0.48f, 0.0f, 0.0f,
		0.49f, 0.0f, 0.0f,

		0.50f, 0.0f, 0.0f,
		0.51f, 0.0f, 0.0f,
		0.52f, 0.0f, 0.0f,
		0.53f, 0.0f, 0.0f,
		0.54f, 0.0f, 0.0f,
		0.55f, 0.0f, 0.0f,
		0.56f, 0.0f, 0.0f,
		0.57f, 0.0f, 0.0f,
		0.58f, 0.0f, 0.0f,
		0.59f, 0.0f, 0.0f,

		0.60f, 0.0f, 0.0f,
		0.61f, 0.0f, 0.0f,
		0.62f, 0.0f, 0.0f,
		0.63f, 0.0f, 0.0f,
		0.64f, 0.0f, 0.0f,
		0.65f, 0.0f, 0.0f,
		0.66f, 0.0f, 0.0f,
		0.67f, 0.0f, 0.0f,
		0.68f, 0.0f, 0.0f,
		0.69f, 0.0f, 0.0f,

		0.70f, 0.0f, 0.0f,
		0.71f, 0.0f, 0.0f,
		0.72f, 0.0f, 0.0f,
		0.73f, 0.0f, 0.0f,
		0.74f, 0.0f, 0.0f,
		0.75f, 0.0f, 0.0f,
		0.76f, 0.0f, 0.0f,
		0.77f, 0.0f, 0.0f,
		0.78f, 0.0f, 0.0f,
		0.79f, 0.0f, 0.0f,

		0.80f, 0.0f, 0.0f,
		0.81f, 0.0f, 0.0f,
		0.82f, 0.0f, 0.0f,
		0.83f, 0.0f, 0.0f,
		0.84f, 0.0f, 0.0f,
		0.85f, 0.0f, 0.0f,
		0.86f, 0.0f, 0.0f,
		0.87f, 0.0f, 0.0f,
		0.88f, 0.0f, 0.0f,
		0.89f, 0.0f, 0.0f,

		0.90f, 0.0f, 0.0f,
		0.91f, 0.0f, 0.0f,
		0.92f, 0.0f, 0.0f,
		0.93f, 0.0f, 0.0f,
		0.94f, 0.0f, 0.0f,
		0.95f, 0.0f, 0.0f,
		0.96f, 0.0f, 0.0f,
		0.97f, 0.0f, 0.0f,
		0.98f, 0.0f, 0.0f,
		0.99f, 0.0f, 0.0f,

		1.00f, 0.0f, 0.0f
	};

	float lines_gradients[101 + 1 + 9 - 1] = {
		10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,
		10,10,
		10,24,36,50,63,77,92,109
	};

    // >>> Different ways of drawing lines
    visual.send_lines(111, &myLines[0][0]);

    visual.send_lines(111, &myLines[0][0], &lines_categories[0], categories);        // Possible flags: CATEGORIES, COLORS, GRADIENT.
    visual.send_lines(111, &myLines[0][0], &lines_categories[0]);                    // By default, interpreted as CATEGORIES

    visual.send_lines(111, &myLines[0][0], &lines_colors[0][0], colors);

    visual.send_palette(&lines_gradient_palette[0][0], 101, lines);
    visual.send_lines(111, &myLines[0][0], lines_gradients, gradient, 10, 109);
    visual.send_lines(111, &myLines[0][0], lines_gradients, gradient, 0, 0);


	// Send data to the "data window" --------------
	std::string additional_data[10] = 
	{
		"You can send data to this \"data window\"",
		"You only have to call the method \"fill_data_window\"",
		"Pass a pointer to an array of 10 strings to it",
		"The method will publish one string per line",
		"The empty strings (=\"\") won't be published",
		"", "", "", "", ""
	};

	visual.fill_data_window(additional_data);

    //std::this_thread::sleep_for(std::chrono::seconds(60));
    std::cin >> i;
    return 0;
}
