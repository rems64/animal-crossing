#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

// clang-format off
// order matters here
#include "mem.hpp"
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
// clang-format on
#include <glm/glm.hpp>
#include <vector>

void init_glfw();
void init_glad();

void check_shader_compile_status(unsigned int shader);
void check_program_link_status(unsigned int program);

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    static void set_vertex_attrib_pointer();
};

class VertexArray {
public:
    VertexArray(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    void bind();
    void unbind();

    // clang-format off
    inline size_t get_indices_count() const { return m_indices_count; }
    // clang-format on

private:
    unsigned int m_vao;
    size_t m_indices_count = 0;
};

class Shader {
public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);

    void bind();

    template<typename T> void set_uniform(const char *name, const T &value);

private:
    unsigned int m_program;
};

class Drawable {
public:
    Drawable() = default;
    virtual ~Drawable(){};
    virtual void draw() = 0;
};

class Renderer {
public:
    Renderer(){};

    void register_drawable(Drawable &drawable);
    void render();

private:
    std::vector<Drawable *> m_drawables = {};
};

#endif
