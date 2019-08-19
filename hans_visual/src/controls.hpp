#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Window size
#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   768

// Camera system
#define FPS     1
#define SPHERE  2

class controls;
extern controls camera;

class controls {
    // Time
    float lastTime = 0.0;
    float currentTime;
    float deltaTime;

    // Mouse
    double xpos, ypos;
    double xpos0, ypos0;
    bool L_pressed, R_pressed;

    // camera parameters
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    float initialFoV;
    float speed;
    float mouseSpeed;
    glm::vec3 sphere_center;
    float radius;
    float scroll_speed;
    float minimum_radius;

    int camera_mode;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    void computeMatricesFromInputs_FPS(GLFWwindow*);
    void computeMatricesFromInputs_spherical(GLFWwindow*);

public:
    controls(int mode = FPS);
    void adjustments(GLFWwindow *window);
    void computeMatricesFromInputs(GLFWwindow* window);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    friend void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    friend void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
};

// Callback functions: Get mouse click - Get mouse out/in window - Get cursor position
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

#endif
