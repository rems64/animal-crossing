#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "glm/trigonometric.hpp"
#include "mem.hpp"
#include "mesh.hpp"

class Transform {
public:
    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_scale(glm::vec3 scale);

    // clang-format off
    inline glm::vec3 get_position() { return m_position; };
    inline glm::vec3 get_rotation() { return m_rotation; };
    inline glm::vec3 get_scale() { return m_scale; };
    // clang-format on
    glm::mat4 get();

private:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};

private:
    glm::mat4 m_matrix = glm::mat4(1.0f);
    bool m_dirty = true;
};

class SceneElement {
public:
    SceneElement();
    SceneElement(Transform transform);

    void set_parent(SceneElement &parent);
    void add_child(SceneElement &child);
    void remove_child(SceneElement &child);

    void update_model_matrix();

    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_scale(glm::vec3 scale);

    // clang-format off
    inline glm::mat4 get_model_matrix() { return m_model_matrix; };
    inline glm::vec3 get_position() { return m_transform.get_position(); };
    inline glm::vec3 get_rotation() { return m_transform.get_rotation(); };
    inline glm::vec3 get_scale() { return m_transform.get_scale(); };
    // clang-format on

protected:
    SceneElement *m_parent = nullptr;
    std::vector<SceneElement *> m_children;
    Transform m_transform;

private:
    glm::mat4 m_model_matrix = glm::mat4(1.f);
};

class MeshComponent : public SceneElement {
public:
    MeshComponent(Mesh mesh, Shader &shader);

    Mesh &get_mesh() {
        return m_mesh;
    };

    // clang-format off
    inline DrawableMesh &get_drawable() { return m_drawable_mesh; }
    // clang-format on

private:
    Mesh m_mesh;
    DrawableMesh m_drawable_mesh;
};

class Camera {
public:
    Camera();

    glm::mat4 get_projection();

private:
    float m_fov = glm::radians(60.f);
    float m_near = .1f;
    float m_far = 100.f;
};

class CameraComponent : public SceneElement {
public:
    CameraComponent(Camera camera);

    // clang-format off
    inline glm::mat4 get_view() { return glm::inverse(get_model_matrix()); }
    inline glm::mat4 get_projection() { return m_camera.get_projection(); }
    // clang-format on

private:
    Camera m_camera;
};

class CameraBoom : public SceneElement {
public:
    CameraBoom(Camera camera, float distance);

    // clang-format off
    inline ref<CameraComponent> get_camera() const { return m_camera_component; }
    // clang-format on

private:
    ref<CameraComponent> m_camera_component;
};

#endif  // SCENE_HPP
