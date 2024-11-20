#include "mesh.hpp"
#include "globals.hpp"
#include "graphics.hpp"
#include "scene.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    : m_vertices(vertices), m_indices(indices) {}

static std::vector<std::string> split(std::string str, char delimiter = ' ') {
    std::vector<std::string> vec;
    std::stringstream ss(str);
    std::string tok;
    while (std::getline(ss, tok, delimiter))
        vec.push_back(tok);
    return vec;
}

Mesh load_mesh(const std::string &filepath) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        throw std::runtime_error("failed to load mesh");
    }

    aiMesh *mesh = scene->mMeshes[0];

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back({
            .position = glm::vec3(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            .normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
        });
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}

DrawableMesh::DrawableMesh(Mesh &mesh, Shader &shader, MeshComponent &mesh_component)
    : m_vao(mesh.get_vertices(), mesh.get_indices()),
      m_shader(&shader),
      m_mesh_component(&mesh_component) {}

void DrawableMesh::draw() {
    m_shader->bind();
    m_shader->set_uniform<glm::mat4>("model", m_mesh_component->get_model_matrix());
    m_shader->set_uniform<glm::mat4>("view", G.camera->get_view());
    m_shader->set_uniform<glm::mat4>("projection", G.camera->get_projection());

    m_vao.bind();

    glDrawElements(GL_TRIANGLES, m_vao.get_indices_count(), GL_UNSIGNED_INT, 0);
}
