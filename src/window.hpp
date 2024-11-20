#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <functional>
#include <optional>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
public:
    Window();
    Window(const char *name, glm::ivec2 size);

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&other) noexcept;
    Window &operator=(Window &&other) noexcept;

    ~Window();

    bool is_valid() const;
    bool should_close();

    void setup_callbacks(GLFWcursorposfun mouse_cb,
                         GLFWmousebuttonfun mouse_button_cb,
                         GLFWscrollfun mousewheel_cb,
                         GLFWkeyfun key_cb);

    // clang-format off
    inline float get_aspect() { return (float)m_size.x / (float)m_size.y; }
    inline uint32_t get_width() const { return m_size.x; }
    inline uint32_t get_height() const { return m_size.y; }
    // clang-format on

    GLFWwindow *get_glfw_window() const;
    void make_current();
    void swap_buffers();
    void destroy();

    static void resize_callback(GLFWwindow *window, int width, int height);
    void set_user_resize_callback(std::function<void(GLFWwindow *, int, int)> function);

protected:
    void create();

private:
    std::optional<std::function<void(GLFWwindow *, int, int)>> m_user_resize = {};
    const char *m_name;
    glm::ivec2 m_size;
    GLFWwindow *m_window;
};

#endif  // WINDOW_HPP
