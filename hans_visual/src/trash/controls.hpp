#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs_FPS(GLFWwindow*);
void computeMatricesFromInputs_spherical(GLFWwindow*);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif