#include "window.hpp"
#include "glm/fwd.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <stdexcept>

// Window::Window() : m_name("Default Window"), m_size(glm::ivec2(800, 600)), m_window(nullptr) {
//     create();
// }

Window::Window() : Window("Game", glm::ivec2(512, 512)) {}

Window::Window(const char *name, glm::ivec2 size) : m_name(name), m_size(size), m_window(nullptr) {
    create();
}

Window::Window(Window &&other) noexcept
    : m_name(other.m_name), m_size(other.m_size), m_window(other.m_window) {
    other.m_window = nullptr;
}

Window &Window::operator=(Window &&other) noexcept {
    if (this != &other) {
        destroy();

        m_name = other.m_name;
        m_size = other.m_size;
        m_window = other.m_window;

        other.m_window = nullptr;
    }
    return *this;
}

Window::~Window() {}

void Window::create() {
    m_window = glfwCreateWindow(m_size.x, m_size.y, m_name, nullptr, nullptr);

    if (!is_valid()) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        throw std::runtime_error("failed to create a glfw window");
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, Window::resize_callback);
}

void Window::resize_callback(GLFWwindow *window, int width, int height) {
    Window *window_object = (Window *)glfwGetWindowUserPointer(window);
    if (!window_object)
        return;
    window_object->m_size = glm::vec2(width, height);

    if (window_object->m_user_resize.has_value())
        window_object->m_user_resize.value()(window, width, height);
}

void Window::destroy() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::is_valid() const {
    return m_window != nullptr;
}

void Window::setup_callbacks(GLFWcursorposfun mouse_cb,
                             GLFWmousebuttonfun mouse_button_cb,
                             GLFWscrollfun mousewheel_cb,
                             GLFWkeyfun key_cb) {
    glfwSetCursorPosCallback(m_window, mouse_cb);
    glfwSetMouseButtonCallback(m_window, mouse_button_cb);
    glfwSetScrollCallback(m_window, mousewheel_cb);
    glfwSetKeyCallback(m_window, key_cb);
}

void Window::set_user_resize_callback(std::function<void(GLFWwindow *, int, int)> function) {
    m_user_resize = function;
}

bool Window::should_close() {
    return glfwWindowShouldClose(m_window);
}

GLFWwindow *Window::get_glfw_window() const {
    return m_window;
}

void Window::make_current() {
    glfwMakeContextCurrent(m_window);
}

void Window::swap_buffers() {
    glfwSwapBuffers(m_window);
}
