// http://csharphelper.com/blog/2015/12/build-a-geodesic-sphere-with-wpf-and-c/

#include "visualizerClass.hpp"
#include <cmath>
#include <chrono>

// Pass a pointer of an array[12][3] to store the icosahedron vertices
void icosahedron(float side_length, float(*points)[3]);

int main() {

	visualizerClass visual;
    visual.open_window();                                   // Open a visualizer window

	// Draw points ------------------------------
    float pnts[12][3];
    icosahedron(2, &pnts[0]);						// Fill the icos[] array with points coordinates (in this case 12 points, with 3 coordinates each)
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
    float points_gradients[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
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
    float cubes_gradients[3] = {1,6,12};
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

    // >>> Different ways of drawing lines
    visual.send_lines(111, &myLines[0][0]);

    visual.send_lines(111, &myLines[0][0], &lines_categories[0], categories);        // Possible flags: CATEGORIES, COLORS, GRADIENT.
    visual.send_lines(111, &myLines[0][0], &lines_categories[0]);                    // By default, interpreted as CATEGORIES

    visual.send_lines(111, &myLines[0][0], &lines_colors[0][0], colors);

    //visual.send_palette(&cubes_gradient_palette[0][0], 21, lines);
    //visual.send_lines(111, &myLines[0][0], cubes_gradients, gradient, 1, 12);
    //visual.send_lines(111, &myLines[0][0], cubes_gradients, gradient, 0, 0);


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

void icosahedron(float side_length, float(*points)[3]) {

	float S = side_length;
	const float pi = 3.14159265359;
	float t1 = 2 * pi / 5;
	float t2 = (pi / 2) - t1;
	float t4 = t1 / 2;
	float t3 = t4 - pi / 10;

	float R = (S / 2) / std::sin(t4);
	float H = std::cos(t4) * R;
	float Cx = R * std::sin(t2);
	float Cz = R * std::cos(t2);
	float H1 = std::sqrt(S * S - R * R);
	float H2 = std::sqrt((H + R) * (H + R) - H * H);
	float Y2 = (H2 - H1) / 2;
	float Y1 = Y2 + H1;

	points[0][0] = 0.;
	points[0][1] = Y1;
	points[0][2] = 0.;

	points[1][0] = R;
	points[1][1] = Y2;
	points[1][2] = 0.;

	points[2][0] = Cx;
	points[2][1] = Y2;
	points[2][2] = Cz;

	points[3][0] = -H;
	points[3][1] = Y2;
	points[3][2] = S / 2;

	points[4][0] = -H;
	points[4][1] = Y2;
	points[4][2] = -S / 2;

	points[5][0] = Cx;
	points[5][1] = Y2;
	points[5][2] = -Cz;

	points[6][0] = -R;
	points[6][1] = -Y2;
	points[6][2] = 0.;

	points[7][0] = -Cx;
	points[7][1] = -Y2;
	points[7][2] = -Cz;

	points[8][0] = H;
	points[8][1] = -Y2;
	points[8][2] = -S / 2;

	points[9][0] = H;
	points[9][1] = -Y2;
	points[9][2] = S / 2;

	points[10][0] = -Cx;
	points[10][1] = -Y2;
	points[10][2] = Cz;

	points[11][0] = 0.;
	points[11][1] = -Y1;
	points[11][2] = 0.;
}
