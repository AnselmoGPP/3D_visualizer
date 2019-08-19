
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

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



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
    //GLuint programID = LoadShaders(	"//home//hank//dev//OGL//hans_visualizer//hans_visual//src//TransformVertexShader.vertexshader", "//home//hank//dev//OGL//hans_visualizer//hans_visual//src//ColorFragmentShader.fragmentshader");
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
        cam.computeMatricesFromInputs(window);
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

        // Points ----------------------------------------------

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
            4,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        //glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
        glDrawArrays(GL_POINTS, 0, points_to_print);

        // Cubes ----------------------------------------------

        mut_cubes.lock();
        glBindBuffer(GL_ARRAY_BUFFER, cubesbuffer);
        glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 3 * sizeof(float), &cubes_buffer[0][0][0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, cubescolors);
        glBufferData(GL_ARRAY_BUFFER, cubes_to_print * 12 * 3 * 4 * sizeof(float), &cubes_colors_buffer[0][0][0], GL_DYNAMIC_DRAW);
        mut_cubes.unlock();

        // 1st attribute buffer : cubes
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubesbuffer);
        glVertexAttribPointer(
            0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubescolors);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            4,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        glDisable(GL_CULL_FACE);
        glDrawArrays(GL_TRIANGLES, 0, cubes_to_print * 12 * 3);
        glEnable(GL_CULL_FACE);

        // Lines --------------------------------------------------

        mut_lines.lock();
        glBindBuffer(GL_ARRAY_BUFFER, linesbuffer);
        glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 3 * sizeof(float), &lines_buffer[0][0][0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, linescolors);
        glBufferData(GL_ARRAY_BUFFER, lines_to_print * 2 * 4 * sizeof(float), &lines_colors_buffer[0][0], GL_DYNAMIC_DRAW);
        mut_lines.unlock();

        // 1st attribute buffer : cubes
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, linesbuffer);
        glVertexAttribPointer(
            0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, linescolors);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            4,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        glDrawArrays(GL_LINES, 0, lines_to_print * 2);

        // ------------------------------------------

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);



        // ImGui window
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            if (ImGui::Button("Button")) counter++;                 // Buttons return true when clicked (most widgets return true when edited/activated)
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        // ImGui rendering
        ImGui::Render();
        //int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



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
