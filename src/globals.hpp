#pragma once

#include "scene.hpp"
#include "window.hpp"

struct Input {
    bool mouse_pressed = false;
    glm::vec2 mouse_position = glm::vec2(0);
};

struct Screen {
    uint32_t width;
    uint32_t height;
    bool resized = true;
};

struct Globals {
    const char *name = "Game";
    ref<Window> main_window = nullptr;
    Screen screen = {};
    Input input = {};
    ref<Renderer> renderer;
    ref<CameraComponent> camera;
    float target_fps = 60.f;
};

extern Globals G;
