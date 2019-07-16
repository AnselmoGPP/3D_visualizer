
#include "visualizerClass.hpp"

int visualizerClass::run_thread() {

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

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, ">>> Anselmo visualizer <<<", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);			// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Hide the mouse and enable unlimited movement

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);		// Dark blue background

	glEnable(GL_DEPTH_TEST);					// Enable depth test
	glDepthFunc(GL_LESS);						// Accept fragment if it closer to the camera than the former one

	glEnable(GL_CULL_FACE);						// Cull triangles which normal is not towards the camera

	glEnable(GL_PROGRAM_POINT_SIZE);			// Enable GL_POINTS
    glPointSize(10.0);



	// Vertex array object (VAO)
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
//    	GLuint programID = LoadShaders(	"..//src//TransformVertexShader.vertexshader",
//                                    "..//src//ColorFragmentShader.fragmentshader");
    	GLuint programID = LoadShaders(	"//home//hank//dev//OGL//hans_visualizer//hans_visual//src//TransformVertexShader.vertexshader",
                                    	"//home//hank//dev//OGL//hans_visualizer//hans_visual//src//ColorFragmentShader.fragmentshader");

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

// GL_DYNAMIC_DRAW ---------------------------------------------------
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);



	do {
		mut.lock();
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, arr_size * sizeof(float), points_buffer, GL_DYNAMIC_DRAW);		// arr_size * sizeof(float)

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, arr_size * sizeof(float), colors_buffer, GL_DYNAMIC_DRAW);
		mut.unlock();
// ------------------------------------------------------------------

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		glDrawArrays(GL_POINTS, 0, arr_size/3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);



	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	//glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
