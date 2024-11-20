#ifndef MESH_HPP
#define MESH_HPP

#include "graphics.hpp"
#include <vector>

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    // clang-format off
    std::vector<Vertex> &get_vertices() { return m_vertices; };
    std::vector<unsigned int> &get_indices() { return m_indices; };
    // clang-format on

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};

Mesh load_mesh(const std::string &filepath);

class MeshComponent;

class DrawableMesh : public Drawable {
public:
    DrawableMesh(Mesh &mesh, Shader &shader, MeshComponent &mesh_component);
    ~DrawableMesh(){};

    void draw() override;

private:
    VertexArray m_vao;

    MeshComponent *m_mesh_component;
    Shader *m_shader = nullptr;
};

#endif  // MESH_HPP
