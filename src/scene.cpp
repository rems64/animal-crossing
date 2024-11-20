#include "scene.hpp"
#include "globals.hpp"
#include "mesh.hpp"

glm::mat4 Transform::get() {
    if (m_dirty) {
        m_dirty = false;
        glm::mat4 rotation_matrix = glm::mat4(1.f);
        rotation_matrix = glm::rotate(rotation_matrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        rotation_matrix = glm::rotate(rotation_matrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation_matrix = glm::rotate(rotation_matrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

        // Order matters too
        m_matrix = glm::translate(glm::mat4(1.0f), m_position) * rotation_matrix *
                   glm::scale(glm::mat4(1.0f), m_scale);
    }
    return m_matrix;
}

void Transform::set_position(glm::vec3 position) {
    m_position = position;
    m_dirty = true;
}
void Transform::set_rotation(glm::vec3 rotation) {
    m_rotation = rotation;
    m_dirty = true;
}
void Transform::set_scale(glm::vec3 scale) {
    m_scale = scale;
    m_dirty = true;
}

SceneElement::SceneElement() : SceneElement(Transform{}) {
    update_model_matrix();
}

SceneElement::SceneElement(Transform transform) : m_transform(transform) {}

void SceneElement::set_parent(SceneElement &parent) {
    if (m_parent) {
        m_parent->remove_child(*this);
    }
    m_parent = &parent;
    m_parent->add_child(*this);
    update_model_matrix();
}

void SceneElement::add_child(SceneElement &child) {
    m_children.push_back(&child);
}

void SceneElement::remove_child(SceneElement &child) {
    for (auto it = m_children.begin(); it != m_children.end(); it++) {
        if (*it == &child) {
            m_children.erase(it);
            return;
        }
    }
}

void SceneElement::update_model_matrix() {
    if (m_parent)
        m_model_matrix = m_parent->m_model_matrix * m_transform.get();
    else
        m_model_matrix = m_transform.get();

    for (size_t i = 0; i != m_children.size(); i++) {
        m_children[i]->update_model_matrix();
    }
}

void SceneElement::set_position(glm::vec3 position) {
    m_transform.set_position(position);
    update_model_matrix();
};

void SceneElement::set_rotation(glm::vec3 rotation) {
    m_transform.set_rotation(rotation);
    update_model_matrix();
};

void SceneElement::set_scale(glm::vec3 scale) {
    m_transform.set_scale(scale);
    update_model_matrix();
};

MeshComponent::MeshComponent(Mesh mesh, Shader &shader)
    : SceneElement(), m_mesh(mesh), m_drawable_mesh(mesh, shader, *this) {}

Camera::Camera() {}

glm::mat4 Camera::get_projection() {
    return glm::perspective(m_fov, G.main_window->get_aspect(), m_near, m_far);
};

CameraComponent::CameraComponent(Camera camera) : SceneElement(), m_camera(camera) {}

CameraBoom::CameraBoom(Camera camera, float distance) : SceneElement() {
    m_camera_component = std::make_shared<CameraComponent>(camera);
    m_camera_component->set_parent(*this);
    m_camera_component->set_position(glm::vec3(0.f, -distance, 0.f));
    m_camera_component->set_rotation(
        glm::vec3(glm::pi<float>() / 2.f, 0.f, 0.f));
}
