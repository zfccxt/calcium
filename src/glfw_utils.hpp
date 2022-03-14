#pragma once

struct GLFWmonitor;
struct GLFWwindow;

namespace cl::glfw {

GLFWmonitor* FindBestMonitor(GLFWwindow* glfw_window);
void CenterWindow(GLFWwindow* glfw_window, bool center_horizontal = true, bool center_vertical = true);

void GlfwInit();
void GlfwShutdown();

}
