
// http://csharphelper.com/blog/2015/12/build-a-geodesic-sphere-with-wpf-and-c/

#include "visualizerClass.hpp"
#include <cmath>
#include <chrono>

// Pass a pointer of an array[12][3] to store the icosahedron vertices
void icosahedron(float side_length, float(*points)[3]);
// This represents the function [ y(x) = a + bx + cx^2 + dx^3 ]. It outputs the y value for the inclusive range [xmin, xmax] (including extremes)
void pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d);

int main(){

	visualizerClass visual;
	visual.run();                                   // Open a visualizer window

	// Draw points ------------------------------
    float icos[12][3];
    icosahedron(2, &icos[0]);						// Fill the icos[] array with points coordinates (in this case 12 points, with 3 coordinates each)

    visual.send_data_as_array(12, &icos[0][0]);     // Load the points on the visualizer. You can also load them from a vector<vector<pnt3D>> using:   visual.send_data_as_vector(cluster_set);

	// Draw cubes -------------------------------
    cube3D myCubes[3] = {
            cube3D(0, 0, 0, 1, 1, 1, 0.0*3.1415, 0),
            cube3D(1, 1, 1, 2, 1, 0.5, 0.3*3.1415, 0),
            cube3D(1, -1, 0, 1, 0.5, 0.1, 0.6*3.1415, 0)
    };

    visual.send_cubes(2, myCubes);                  // Load boxes on the visualizer

	// Draw lines -------------------------------
	float join_points[9][3] = {
		-3,  3,  3,
		-3, -3,  3,
		 3, -3,  3,
		 3,  3,  3,
		 3,  3, -3,
		 3, -3, -3,
		-3, -3, -3,
		-3,  3, -3,
		-3,  3,  3
	};
	
	visual.send_lines(9, &join_points[0][0]);					// Load points that will be used to draw lines

	float pol_3th[101][3];
	pol_3th_degree(&pol_3th[0][0], -10, 10, 100, 0, 1, 1, 0);	// Fill the pol_3th array 
	
	visual.send_lines(101, &pol_3th[0][0]);						// This erases the previous lines-buffer and draws a new one

    std::this_thread::sleep_for(std::chrono::seconds(60));
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

void pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d) {

	float step = (xmax - xmin) / sample;
	int counter = 0;
	for (int i = 0; i <= sample; i++)
	{
		results_array[i * 3 + 0] = xmin;
		results_array[i * 3 + 1] = a + b * xmin + c * xmin*xmin + d * xmin*xmin*xmin;
		results_array[i * 3 + 2] = 0;

		xmin += step;
	}
}