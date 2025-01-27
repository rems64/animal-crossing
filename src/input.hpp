#pragma once

#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void mousewheel_callback(GLFWwindow *window, double wheel_x, double wheel_y);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
