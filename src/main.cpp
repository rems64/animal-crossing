#include "glm/trigonometric.hpp"
#include "globals.hpp"
#include "graphics.hpp"
#include "mesh.hpp"
#include "primitives.hpp"
#include "scene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

Globals G;

void mouse_move(GLFWwindow *window, double xpos, double ypos) {
    glm::vec2 new_position = glm::vec2(xpos, ypos);
    glm::vec2 delta = new_position - G.input.mouse_position;
    G.input.mouse_position = new_position;
    if (G.input.mouse_pressed) {
        // G.camera.theta = glm::clamp(
        //     G.camera.theta - 0.01f * delta.y, -glm::pi<float>() / 2, glm::pi<float>() / 2);
        // G.camera.phi -= 0.01f * delta.x;
    }
}

void mouse_press(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            G.input.mouse_pressed = action == GLFW_PRESS;
            break;
        default:;
    }
}

void on_window_resize(GLFWwindow *window, int width, int height) {
    if (window == G.main_window->get_glfw_window()) {
        G.screen.width = width;
        G.screen.height = height;
        G.screen.resized = true;
    }
}

void mouse_scroll(GLFWwindow *window, double xoffset, double yoffset) {}

void keypress(GLFWwindow *window, int key, int scancode, int action, int mods) {}

int main(int argc, char **argv) {
    init_glfw();

    std::shared_ptr<Window> window = std::make_shared<Window>("Main Window", glm::ivec2(512, 512));

    ref<Renderer> renderer = std::make_shared<Renderer>();

    // clang-format off
    G = Globals {
        .name = "Game",
        .main_window = window,
        .screen = {
            .width = window->get_width(),
            .height = window->get_height()
        },
        .renderer = renderer,
        .target_fps = 60.f,
    };
    // clang-format on

    window->setup_callbacks(mouse_move, mouse_press, mouse_scroll, keypress);
    window->set_user_resize_callback(on_window_resize);

    window->make_current();
    init_glad();

    const double frame_time = 1.0 / G.target_fps;
    double last_frame = glfwGetTime();

    glClearColor(0.1f, 0.11f, 0.13f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    Shader shader = Shader("shaders/basic.vert", "shaders/basic.frag");

    SceneElement root;
    MeshComponent ground(generate_sphere(20, 20, 1.f), shader);
    MeshComponent monkey(load_mesh("models/monkey.obj"), shader);
    ground.set_parent(root);
    monkey.set_parent(root);
    float radius = 10;
    ground.set_position(glm::vec3(0.f, 0.f, -radius));
    ground.set_scale(radius * glm::vec3(1.f, 1.f, 1.f));
    monkey.set_position(glm::vec3(1.f, 0.f, 0.f));

    renderer->register_drawable(ground.get_drawable());
    renderer->register_drawable(monkey.get_drawable());

    ref<CameraBoom> camera_boom = std::make_shared<CameraBoom>(Camera(), 5.f);
    G.camera = camera_boom->get_camera();
    camera_boom->set_rotation(glm::vec3(glm::radians(-10.f), 0.f, 0.f));

    while (!window->should_close()) {
        // Begin frame
        double current_time = glfwGetTime();
        double delta_time = current_time - last_frame;
        last_frame = current_time;

        monkey.set_rotation(glm::vec3(0.f, 0.f, current_time));

        // Render frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (G.screen.resized) {
            glViewport(0, 0, G.screen.width, G.screen.height);
        }

        renderer->render();

        // End frame
        glfwPollEvents();

        if (G.target_fps > 0) {
            double frame_end = glfwGetTime();
            double wait_time = frame_time - (frame_end - current_time);
            if (wait_time > 0) {
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = (long)(wait_time * 1e9);
                nanosleep(&ts, nullptr);
            }
        }

        window->swap_buffers();
    }

    window->destroy();
    glfwTerminate();
    return 0;
}
