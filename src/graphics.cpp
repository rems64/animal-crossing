#include "graphics.hpp"
#include "file_utils.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstddef>
#include <iostream>

void init_glfw() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        const char *description;
        glfwGetError(&description);
        std::cerr << "\tdescription : " << description << std::endl;
        throw std::runtime_error("failed to initialize glfw");
    }
}

void init_glad() {
    // init_glad requires an active context

    int glad_status = gladLoadGL();
    if (!glad_status) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::cerr << "\tstatus : " << glad_status << std::endl;

        // Try to get error information from GLFW
        const char *description;
        int code = glfwGetError(&description);
        if (description) {
            std::cerr << "GLFW Error (" << code << "): " << description << std::endl;
        }

        throw std::runtime_error("failed to load glad");
    }
}

void check_shader_compile_status(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed" << std::endl;
        std::cerr << "\t" << infoLog << std::endl;
    }
}

void check_program_link_status(unsigned int program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed" << std::endl;
        std::cerr << "\t" << infoLog << std::endl;
    }
}

void Vertex::set_vertex_attrib_pointer() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
}

VertexArray::VertexArray(const std::vector<Vertex> &vertices,
                         const std::vector<unsigned int> &indices) : m_indices_count(indices.size()) {
    unsigned int vbo, ebo;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);
    Vertex::set_vertex_attrib_pointer();

    unbind();
}

void VertexArray::bind() {
    glBindVertexArray(m_vao);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {
    std::string vertex_shader_content = read_file(vertex_path);
    std::string fragment_shader_content = read_file(fragment_path);

    const char *vertex_shader_source = vertex_shader_content.c_str();
    const char *fragment_shader_source = fragment_shader_content.c_str();

    // Create and compile vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    check_shader_compile_status(vertex_shader);

    // Create and compile fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    check_shader_compile_status(fragment_shader);

    // Create shader program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    glLinkProgram(m_program);
    check_program_link_status(m_program);

    // Clean up shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::bind() {
    glUseProgram(m_program);
}

template<> void Shader::set_uniform<glm::mat4>(const char *name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Renderer::register_drawable(Drawable &drawable) {
    m_drawables.push_back(&drawable);
}

void Renderer::render() {
    for (size_t i=0; i<m_drawables.size(); i++) {
        m_drawables[i]->draw();
    }
}