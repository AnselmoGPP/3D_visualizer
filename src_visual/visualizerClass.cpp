#include "visualizerClass.hpp"

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

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);		// Grey background

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

void visualizerClass::load_points(GLuint vertexbuffer, GLuint colorbuffer){

    mut_points.lock();
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, points_to_print * 3 * sizeof(float), &points_buffer[0][0], GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, points_to_print * 4 * sizeof(float), &points_colors_buffer[0][0], GL_DYNAMIC_DRAW);
    mut_points.unlock();

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

    mut_lines.lock();
    glBindBuffer(GL_ARRAY_BUFFER, linesbuffer);
    glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 3 * sizeof(float), &lines_buffer[0][0][0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, linescolors);
    glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 4 * sizeof(float), &lines_colors_buffer[0][0], GL_DYNAMIC_DRAW);
    mut_lines.unlock();

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

    mut_cubes.lock();
    glBindBuffer(GL_ARRAY_BUFFER, cubesbuffer);
    glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 3 * sizeof(float), &cubes_buffer[0][0][0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cubescolors);
    glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 4 * sizeof(float), &cubes_colors_buffer[0][0][0], GL_DYNAMIC_DRAW);
    mut_cubes.unlock();

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

void visualizerClass::create_windows() {

    std::string label;

	{
		ImGui::Begin("Checkboxes");
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
