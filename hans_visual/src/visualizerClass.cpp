#include "visualizerClass.hpp"

// Public members --------------------------------------------------------

visualizerClass::visualizerClass(){

    float temp_pal[21][3] =
    {
                      {1.0f,	0.0f,	0.00f},
                      {0.0f,	1.0f,	0.00f},
                      {0.0f,	0.0f,	1.00f},
                      {1.0f,	0.0f,	1.00f},
                      {1.0f,	1.0f,	0.00f},
                      {0.0f,	1.0f,	1.00f},
                      {0.5f,	0.0f,	0.00f},
                      {0.0f,	0.5f,	0.00f},
                      {0.0f,	0.0f,	0.50f},
                      {0.5f,	0.5f,	0.00f},
                      {0.5f,	0.0f,	0.50f},
                      {0.0f,	0.5f,	0.50f},
                      {0.75f,   0.25f,	0.25f},
                      {0.25f,   0.75f,	0.25f},
                      {0.25f,   0.25f,	0.75f},
                      {0.25f,   0.25f,	0.00f},
                      {0.25f,   0.0f,	0.25f},
                      {0.0f,	0.25f,	0.25f},
                      {0.75f,   0.75f,	0.00f},
                      {0.0f,	0.75f,	0.75f},
                      {0.75f,   0.0f,	0.75f}
    };

    points_palette = new float[21][3];
    lines_palette  = new float[21][3];
    cubes_palette  = new float[21][3];

    for(int i = 0; i < 21; i++) {
        points_palette[i][0] = temp_pal[i][0];
        points_palette[i][1] = temp_pal[i][1];
        points_palette[i][2] = temp_pal[i][2];

        lines_palette[i][0]  = temp_pal[i][0];
        lines_palette[i][1]  = temp_pal[i][1];
        lines_palette[i][2]  = temp_pal[i][2];

        cubes_palette[i][0]  = temp_pal[i][0];
        cubes_palette[i][1]  = temp_pal[i][1];
        cubes_palette[i][2]  = temp_pal[i][2];
    }
}
visualizerClass::~visualizerClass(){

    delete [] points_palette;
    delete [] lines_palette;
    delete [] cubes_palette;
}

int visualizerClass::open_window() {

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

void visualizerClass::send_points(int number_points, const float *arr, const float *labels, data_buffer array_type, float min, float max) {

    if (number_points > NUM_OF_VERTEX) {
        std::cout << "Too many points for the points-buffer (" << number_points << " > " << NUM_OF_VERTEX << ')' << std::endl;
        number_points = NUM_OF_VERTEX;
    }
    else points_to_print = number_points;

    float siz = max - min;
    int index;

    std::lock_guard<std::mutex> lock_points(mut_points);

    for (int i = 0; i < number_points; i++)
    {
        // Set point position in 3D;
        points_buffer[i][0] = arr[i * 3 + 0];
        points_buffer[i][1] = arr[i * 3 + 1];
        points_buffer[i][2] = arr[i * 3 + 2];

        // Set color of the point
        if		(labels     == nullptr)
        {
            points_colors_buffer[i][0] = 0;
            points_colors_buffer[i][1] = 0;
            points_colors_buffer[i][2] = 0;
            points_colors_buffer[i][3] = points_alpha_channel;
        }
        else if (array_type == categories)
        {
            index = (int)labels[i] % points_palette_size;

            points_colors_buffer[i][0] = points_palette[index][0];
            points_colors_buffer[i][1] = points_palette[index][1];
            points_colors_buffer[i][2] = points_palette[index][2];
            points_colors_buffer[i][3] = points_alpha_channel;
        }
        else if	(array_type == colors)
        {
            points_colors_buffer[i][0] = labels[i * 3 + 0];
            points_colors_buffer[i][1] = labels[i * 3 + 1];
            points_colors_buffer[i][2] = labels[i * 3 + 2];
            points_colors_buffer[i][3] = points_alpha_channel;
        }
        else if (array_type == gradient)
        {
            if      (labels[i] < min) index = 0;
            else if (labels[i] > max) index = points_palette_size - 1;
            else {
                index = (int)((labels[i] - min) * (points_palette_size - 1)) / (int)siz;
            }

            points_colors_buffer[i][0] = points_palette[index][0];
            points_colors_buffer[i][1] = points_palette[index][1];
            points_colors_buffer[i][2] = points_palette[index][2];
            points_colors_buffer[i][3] = points_alpha_channel;
        }
    }
}

void visualizerClass::send_lines(int number_lines, const float *points, const float *labels, data_buffer array_type, float min, float max) {

    lines_to_print = 0;
    int omitted_segments = 0;
    int index = 0;

    std::lock_guard<std::mutex> lock_points(mut_lines);

    // Main loop for filling the class' lines buffer
    for (int i = 0; i < number_lines - 1; i++)
    {
        if (lines_to_print == NUM_OF_LINES) {
            std::cout << "Too many lines for the lines-buffer (" << NUM_OF_LINES << "max.)" << std::endl;
            break;
        }

        // Check for the line jump
        if (points[i * 3 + 3] == 1.2f && points[i * 3 + 4] == 3.4f && points[i * 3 + 5] == 5.6f) {
            i++;
            omitted_segments += 2;
            continue;
        }

        // Save two points (vertex) for each segment
        lines_buffer[i - omitted_segments][0][0] = points[i * 3 + 0];
        lines_buffer[i - omitted_segments][0][1] = points[i * 3 + 1];
        lines_buffer[i - omitted_segments][0][2] = points[i * 3 + 2];

        lines_buffer[i - omitted_segments][1][0] = points[i * 3 + 3];
        lines_buffer[i - omitted_segments][1][1] = points[i * 3 + 4];
        lines_buffer[i - omitted_segments][1][2] = points[i * 3 + 5];

        // Save colors for each vertex
        if		(labels == nullptr)
        {
            lines_colors_buffer[i - omitted_segments][0][0] = 0;
            lines_colors_buffer[i - omitted_segments][0][1] = 0;
            lines_colors_buffer[i - omitted_segments][0][2] = 0;
            lines_colors_buffer[i - omitted_segments][0][3] = lines_alpha_channel;

            lines_colors_buffer[i - omitted_segments][1][0] = 0;
            lines_colors_buffer[i - omitted_segments][1][1] = 0;
            lines_colors_buffer[i - omitted_segments][1][2] = 0;
            lines_colors_buffer[i - omitted_segments][1][3] = lines_alpha_channel;
        }
        else if (array_type == categories)  // labels contains the category of each segment (including line jumps)
        {
            index = (int)labels[i] % lines_palette_size;

            lines_colors_buffer[i - omitted_segments][0][0] = lines_palette[index][0];
            lines_colors_buffer[i - omitted_segments][0][1] = lines_palette[index][1];
            lines_colors_buffer[i - omitted_segments][0][2] = lines_palette[index][2];
            lines_colors_buffer[i - omitted_segments][0][3] = lines_alpha_channel;

            lines_colors_buffer[i - omitted_segments][1][0] = lines_palette[index][0];
            lines_colors_buffer[i - omitted_segments][1][1] = lines_palette[index][1];
            lines_colors_buffer[i - omitted_segments][1][2] = lines_palette[index][2];
            lines_colors_buffer[i - omitted_segments][1][3] = lines_alpha_channel;
        }
        else if	(array_type == colors)      // labels contains the color of each segment (including line jumps)
        {
            lines_colors_buffer[i - omitted_segments][0][0] = labels[i * 3 + 0];
            lines_colors_buffer[i - omitted_segments][0][1] = labels[i * 3 + 1];
            lines_colors_buffer[i - omitted_segments][0][2] = labels[i * 3 + 2];
            lines_colors_buffer[i - omitted_segments][0][3] = lines_alpha_channel;

            lines_colors_buffer[i - omitted_segments][1][0] = labels[i * 3 + 0];
            lines_colors_buffer[i - omitted_segments][1][1] = labels[i * 3 + 1];
            lines_colors_buffer[i - omitted_segments][1][2] = labels[i * 3 + 2];
            lines_colors_buffer[i - omitted_segments][1][3] = lines_alpha_channel;
        }
        else if (array_type == gradient)
        {

        }

        ++lines_to_print;
    }
}

void visualizerClass::send_cubes(int number_cubes, const cube3D *arr, const float *labels, data_buffer array_type, float min, float max) {

    if (number_cubes > NUM_OF_CUBES) {
        std::cout << "Too many cubes for the cubes-buffer (" << number_cubes << " > " << NUM_OF_CUBES << ')' << std::endl;
        cubes_to_print = NUM_OF_CUBES;
    }
    else cubes_to_print = number_cubes;

    float X, Y, Z, x, y, z, rot_H, rot_V;

    std::lock_guard<std::mutex> lock_points(mut_cubes);

    // Fill cubes_buffer                            >>> This draws triangles anti-clockwise
    for (size_t i = 0; i < cubes_to_print; i++) {

        X = arr[i].X;
        Y = arr[i].Y;
        Z = arr[i].Z;
        x = arr[i].width / 2;
        y = arr[i].height / 2;
        z = arr[i].length / 2;
        rot_H = arr[i].rot_H;
        rot_V = arr[i].rot_V;
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
        float x1 = -x, y1 = Y + y, z1 = +z;
        float x2 = -x, y2 = Y - y, z2 = +z;
        float x3 = +x, y3 = Y + y, z3 = +z;
        float x4 = +x, y4 = Y - y, z4 = +z;
        float x5 = +x, y5 = Y + y, z5 = -z;
        float x6 = +x, y6 = Y - y, z6 = -z;
        float x7 = -x, y7 = Y + y, z7 = -z;
        float x8 = -x, y8 = Y - y, z8 = -z;

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
        cubes_buffer[i][9][0] = x4;
        cubes_buffer[i][9][1] = y4;
        cubes_buffer[i][9][2] = z4;
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
    float siz = max - min;
    int index;

    for (int j = 0; j < number_cubes; j++)
    {
        if(labels == nullptr)
        {
            for (int k = 0; k < 12 * 3; k++)    // Go through all the points of all triangles in one box
            {
                cubes_colors_buffer[j][k][0] = 0.1f;
                cubes_colors_buffer[j][k][1] = 0.9f;
                cubes_colors_buffer[j][k][2] = 0.1f;
                cubes_colors_buffer[j][k][3] = cubes_alpha_channel;
            }
        }
        else if(array_type == categories)
        {
            index = (int)labels[j] % cubes_palette_size;

            for (int k = 0; k < 12 * 3; k++)
            {
                cubes_colors_buffer[j][k][0] = cubes_palette[index][0];
                cubes_colors_buffer[j][k][1] = cubes_palette[index][1];
                cubes_colors_buffer[j][k][2] = cubes_palette[index][2];
                cubes_colors_buffer[j][k][3] = cubes_alpha_channel;
            }
        }
        else if(array_type == colors)
        {
            for(int k = 0; k < 12 * 3; k++)
            {
                cubes_colors_buffer[j][k][0] = labels[j * 3 + 0];
                cubes_colors_buffer[j][k][1] = labels[j * 3 + 1];
                cubes_colors_buffer[j][k][2] = labels[j * 3 + 2];
                cubes_colors_buffer[j][k][3] = cubes_alpha_channel;
            }
        }
        else if(array_type == gradient)
        {
            if      (labels[j] < min) index = 0;
            else if (labels[j] > max) index = cubes_palette_size - 1;
            else index = (int)((labels[j] - min) * (cubes_palette_size - 1)) / (int)siz;

            for(int k = 0; k < 12 * 3; k++)
            {
                cubes_colors_buffer[j][k][0] = cubes_palette[index][0];
                cubes_colors_buffer[j][k][1] = cubes_palette[index][1];
                cubes_colors_buffer[j][k][2] = cubes_palette[index][2];
                cubes_colors_buffer[j][k][3] = cubes_alpha_channel;
            }
        }
    }
}

void visualizerClass::send_palette(float *new_palette, int number_colors, object_type obj){

    if(obj == points)
    {
        delete [] points_palette;
        points_palette = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            points_palette[i][0] = new_palette[i * 3 + 0];
            points_palette[i][1] = new_palette[i * 3 + 1];
            points_palette[i][2] = new_palette[i * 3 + 2];
        }

        points_palette_size = number_colors;
    }
    else if(obj == lines)
    {
        delete [] lines_palette;
        lines_palette = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            lines_palette[i][0] = new_palette[i * 3 + 0];
            lines_palette[i][1] = new_palette[i * 3 + 1];
            lines_palette[i][2] = new_palette[i * 3 + 2];
        }

        lines_palette_size = number_colors;
    }
    else if(obj == cubes)
    {
        delete [] cubes_palette;
        cubes_palette = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            cubes_palette[i][0] = new_palette[i * 3 + 0];
            cubes_palette[i][1] = new_palette[i * 3 + 1];
            cubes_palette[i][2] = new_palette[i * 3 + 2];
        }

        cubes_palette_size = number_colors;
    }

}

void visualizerClass::fill_data_window(const std::string *data_strings) {

    std::lock_guard<std::mutex> lock(mut_fill_data);

    for (int i = 0; i < 10; i++)
        data_window[i] = data_strings[i];
}

void visualizerClass::pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d) {

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

// Obsolete functions ----------

void visualizerClass::send_data_as_vector(std::vector<std::vector<pnt3D>>& vecPoints) {

    int color_index = 0;

    std::lock_guard<std::mutex> lock_points(mut_points);

    points_to_print = 0;

    for (int i = 0; i < vecPoints.size(); i++) {
        for (int j = 0; j < vecPoints[i].size(); j++)
        {
            points_buffer[points_to_print][0] = vecPoints[i][j].x;
            points_buffer[points_to_print][1] = vecPoints[i][j].y;
            points_buffer[points_to_print][2] = vecPoints[i][j].z;

            points_colors_buffer[points_to_print][0] = points_palette[color_index][0];
            points_colors_buffer[points_to_print][1] = points_palette[color_index][1];
            points_colors_buffer[points_to_print][2] = points_palette[color_index][2];
            points_colors_buffer[points_to_print][3] = points_alpha_channel;

            points_to_print++;

            if (points_to_print == NUM_OF_VERTEX) {
                std::cout << "Too many points for the points-buffer (" << NUM_OF_VERTEX << " max.)" << std::endl;
                return;
            }
        }
        if (color_index != 20) color_index++;
        else color_index = 0;
    }
}

void visualizerClass::send_data_as_array(int number_points, const float* arr) {

    if (number_points > NUM_OF_VERTEX) {
        std::cout << "Too many points for the points-buffer (" << number_points << " > " << NUM_OF_VERTEX << ')' << std::endl;
        number_points = NUM_OF_VERTEX;
    }
    else points_to_print = number_points;

    std::lock_guard<std::mutex> lock_points(mut_points);

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
}

// Private members -------------------------------------------------------

int visualizerClass::run_thread() {

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, ">>> Anselmo visualizer <<<", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;										// Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }



    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();		//ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);



    cam.adjustments(window);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );		// Background color

    glEnable(GL_DEPTH_TEST);					// Enable depth test
    glDepthFunc(GL_LESS);						// Accept fragment if it closer to the camera than the former one

    glEnable(GL_CULL_FACE);						// Cull triangles which normal is not towards the camera.    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);			// Enable GL_POINTS
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(5.0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0);
    GLfloat lineWidthRange[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);



    // Vertex array object (VAO)
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders				    system("pwd"): /home/hank/dev/OGL/Shaper/_BUILD
    //GLuint programID = LoadShaders(	"//home//hank//src//TransformVertexShader.vertexshader", "//home//hank//src//ColorFragmentShader.fragmentshader");
    GLuint programID = LoadShaders();

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // GL_STATIC_DRAW ---------------------------------------------------
    /*
        // Vertex buffer objects (VBO)
        static const GLfloat g_vertex_buffer_data[] = {
             0.0f,  0.0f,  0.0f,
             1.0f,  0.0f,  0.0f,
             2.0f,  0.0f,  0.0f,
             3.0f,  0.0f,  0.0f,
             4.0f,  0.0f,  0.0f,
             5.0f,  0.0f,  0.0f,
             6.0f,  0.0f,  0.0f,
             7.0f,  0.0f,  0.0f,
             8.0f,  0.0f,  0.0f,
             9.0f,  0.0f,  0.0f,
             10.0f,  0.0f,  0.0f,
             11.0f,  0.0f,  0.0f,
             12.0f,  0.0f,  0.0f,
             13.0f,  0.0f,  0.0f,
             14.0f,  0.0f,  0.0f,
             15.0f,  0.0f,  0.0f,
             16.0f,  0.0f,  0.0f,
             17.0f,  0.0f,  0.0f,
             18.0f,  0.0f,  0.0f,
             19.0f,  0.0f,  0.0f,
             20.0f,  0.0f,  0.0f
        };
        static const GLfloat g_color_buffer_data[] = {
             1.0f,  0.0f,  0.0f,
             0.0f,  1.0f,  0.0f,
             0.0f,  0.0f,  1.0f,
             1.0f,  0.0f,  1.0f,
             1.0f,  1.0f,  0.0f,
             0.0f,  1.0f,  1.0f,
             0.5f,  0.0f,  0.0f,
             0.0f,  0.5f,  0.0f,
             0.0f,  0.0f,  0.5f,
             0.5f,  0.5f,  0.0f,
             0.5f,  0.0f,  0.5f,
             0.0f,  0.5f,  0.5f,
             0.75f,  0.25f,  0.25f,
             0.25f,  0.75f,  0.25f,
             0.25f,  0.25f,  0.75f,
             0.25f,  0.25f,  0.0f,
             0.25f,  0.0f,  0.25f,
             0.0f,  0.25f,  0.25f,
             0.75f,  0.75f,  0.0f,
             0.0f,  0.75f,  0.75f,
             0.75f, 0.0f,  0.75f
        };
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    */
    // ------------------------------------------------------------------

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);

    GLuint cubesbuffer;
    glGenBuffers(1, &cubesbuffer);
    GLuint cubescolors;
    glGenBuffers(1, &cubescolors);

    GLuint linesbuffer;
    glGenBuffers(1, &linesbuffer);
    GLuint linescolors;
    glGenBuffers(1, &linescolors);

    // Main loop --------------------------------------------------------
    do {
        glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        // io.WantCaptureMouse and io.WantCaptureKeyboard flags are true if dear imgui wants to use our inputs (i.e. cursor is hovering a window).
        if (!io.WantCaptureMouse) cam.computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = cam.getProjectionMatrix();
        glm::mat4 ViewMatrix = cam.getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);                     // Identity matrix
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Clear the screen and the depth test
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        load_points(vertexbuffer, colorbuffer);
        load_lines(linesbuffer, linescolors);
        load_cubes(cubesbuffer, cubescolors);



        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);



        // ImGui rendering
        create_windows();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);



        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);



    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteBuffers(1, &cubesbuffer);
    glDeleteBuffers(1, &cubescolors);
    glDeleteBuffers(1, &linesbuffer);
    glDeleteBuffers(1, &linescolors);
    glDeleteProgram(programID);
    //glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void visualizerClass::create_windows() {

    //ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    //ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    //ImGui::Begin("Toolbox");
    //ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Options")) {
                ImGui::MenuItem("Checkboxes", NULL, &show_checkboxes);
                ImGui::MenuItem("Data", NULL, &show_data);
                ImGui::MenuItem("Configuration", NULL, &show_options);
            ImGui::EndMenu();
        }
/*
        if (ImGui::BeginMenu("Camera")) {
            ImGui::MenuItem("Sphere", NULL, &cam_sphere);
            if (cam_sphere) {
                cam_FPS = 0;
                //camera = controls(SPHERE);
                //cam.camera_mode = SPHERE;
            }
            ImGui::MenuItem("FPS", NULL, &cam_FPS);
            if (cam_FPS) {
                cam_sphere = 0;
                //camera = controls(FPS);
                //cam.camera_mode = FPS;
            }
            ImGui::EndMenu();
        }
*/
        ImGui::EndMainMenuBar();
    }

    if(show_checkboxes)
    {
        ImGui::Begin("Checkboxes", &show_checkboxes);
            if (existing_checkboxes[0][0]) ImGui::Checkbox("Points 1", &checkboxes_values[0][0]);
            if (existing_checkboxes[0][1]) ImGui::Checkbox("Points 2", &checkboxes_values[0][1]);
            if (existing_checkboxes[0][2]) ImGui::Checkbox("Points 3", &checkboxes_values[0][2]);
            if (existing_checkboxes[0][3]) ImGui::Checkbox("Points 4", &checkboxes_values[0][3]);
            if (existing_checkboxes[0][4]) ImGui::Checkbox("Points 5", &checkboxes_values[0][4]);
            if (existing_checkboxes[1][0]) ImGui::Checkbox("Lines 1", &checkboxes_values[1][0]);
            if (existing_checkboxes[1][1]) ImGui::Checkbox("Lines 2", &checkboxes_values[1][1]);
            if (existing_checkboxes[1][2]) ImGui::Checkbox("Lines 3", &checkboxes_values[1][2]);
            if (existing_checkboxes[1][3]) ImGui::Checkbox("Lines 4", &checkboxes_values[1][3]);
            if (existing_checkboxes[1][4]) ImGui::Checkbox("Lines 5", &checkboxes_values[1][4]);
            if (existing_checkboxes[2][0]) ImGui::Checkbox("Cubes 1", &checkboxes_values[2][0]);
            if (existing_checkboxes[2][1]) ImGui::Checkbox("Cubes 2", &checkboxes_values[2][1]);
            if (existing_checkboxes[2][2]) ImGui::Checkbox("Cubes 3", &checkboxes_values[2][2]);
            if (existing_checkboxes[2][3]) ImGui::Checkbox("Cubes 4", &checkboxes_values[2][3]);
            if (existing_checkboxes[2][4]) ImGui::Checkbox("Cubes 5", &checkboxes_values[2][4]);
        ImGui::End();
    }

    if (show_data)
    {
        std::lock_guard<std::mutex> lock(mut_fill_data);

        ImGui::Begin("Data window", &show_data);
            for (int i = 0; i < 10; i++) {
                if(data_window[i] != "")
                    ImGui::Text(data_window[i].c_str());
            }
        ImGui::End();
    }

    // Background color, transparencies, show axis,
    if (show_options)
    {
        ImGui::Begin("Options", &show_options);
        ImGui::ColorEdit3("Background", backg_color);
        ImGui::SliderFloat("Points", &points_alpha_channel, 0.0f, 1.0f);
        ImGui::SliderFloat("Lines",  &lines_alpha_channel,  0.0f, 1.0f);
        ImGui::SliderFloat("Boxes",  &cubes_alpha_channel,  0.0f, 1.0f);
        ImGui::End();
        change_alpha_channel(1); change_alpha_channel(2); change_alpha_channel(3);
    }
}

void visualizerClass::create_demo_windows() {

    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float slide = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, world!");
            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::SliderFloat("float", &slide, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Button")) counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
        ImGui::End();
    }

}

void visualizerClass::change_alpha_channel(int object) {

    if (object == 1)	// Points
    {
        for (int i = 0; i < NUM_OF_VERTEX; i++)
            points_colors_buffer[i][3] = points_alpha_channel;
    }
    if (object == 2)	// Lines
    {
        for (int i = 0; i < NUM_OF_LINES; i++)
        {
            lines_colors_buffer[i][0][3] = lines_alpha_channel;
            lines_colors_buffer[i][1][3] = lines_alpha_channel;
        }
    }
    if (object == 3)	// Cubes
    {
        for (int i = 0; i < NUM_OF_CUBES; i++)
            for (int j = 0; j < 12*3; j++)
                cubes_colors_buffer[i][j][3] = cubes_alpha_channel;
    }
}

void visualizerClass::rotation_H(float &x, float &y, float X, float Y, float rot) {

    y = -y;

    float alpha = atan(y / x);
    if (x < 0 && y >= 0) alpha += 3.1415926535f;
    else if (y < 0 && x < 0) alpha += 3.1415926535f;
    else if (y < 0 && x >= 0) alpha += 2.f * 3.1415926535f;

    float hip = sqrt(x * x + y * y);

    float beta = alpha + rot;

    x = hip * cos(beta) + X;
    y = -(hip * sin(beta)) + Y;
}

void visualizerClass::load_points(GLuint vertexbuffer, GLuint colorbuffer){

	{
		std::lock_guard<std::mutex> lock_points(mut_points);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, points_to_print * 3 * sizeof(float), &points_buffer[0][0], GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, points_to_print * 4 * sizeof(float), &points_colors_buffer[0][0], GL_DYNAMIC_DRAW);
	}

    // Bind our texture in Texture Unit 0
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    //glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    //glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
    if(checkboxes_values[0][0])
        glDrawArrays(GL_POINTS, 0, points_to_print);
    else
        glDrawArrays(GL_POINTS, 0, 0);
}

void visualizerClass::load_lines(GLuint linesbuffer, GLuint linescolors){

    {
        std::lock_guard<std::mutex> lock_points(mut_lines);

        glBindBuffer(GL_ARRAY_BUFFER, linesbuffer);
        glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 3 * sizeof(float), &lines_buffer[0][0][0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, linescolors);
        glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 4 * sizeof(float), &lines_colors_buffer[0][0], GL_DYNAMIC_DRAW);
    }

    // 1st attribute buffer : cubes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, linesbuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, linescolors);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    if(checkboxes_values[1][0])
        glDrawArrays(GL_LINES, 0, lines_to_print * 2);
    else
        glDrawArrays(GL_LINES, 0, 0);
}

void visualizerClass::load_cubes(GLuint cubesbuffer, GLuint cubescolors){

    {
        std::lock_guard<std::mutex> lock_points(mut_cubes);

        glBindBuffer(GL_ARRAY_BUFFER, cubesbuffer);
        glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 3 * sizeof(float), &cubes_buffer[0][0][0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, cubescolors);
        glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 4 * sizeof(float), &cubes_colors_buffer[0][0][0], GL_DYNAMIC_DRAW);
    }

    // 1st attribute buffer : cubes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubesbuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cubescolors);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glDisable(GL_CULL_FACE);
    if(checkboxes_values[2][0])
        glDrawArrays(GL_TRIANGLES, 0, cubes_to_print * 12 * 3);
    else
        glDrawArrays(GL_TRIANGLES, 0, 0);
    glEnable(GL_CULL_FACE);
}
