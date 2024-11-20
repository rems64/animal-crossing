#include "primitives.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "graphics.hpp"
#include "mesh.hpp"

Mesh generate_sphere(uint32_t rings, uint32_t segments, float radius) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (size_t ring = 0; ring <= rings; ring++) {
        for (size_t segment = 0; segment <= segments; segment++) {
            const float theta = (float)ring / rings * glm::pi<float>();
            const float phi = (float)segment / segments * glm::pi<float>() * 2.f;
            const glm::vec3 direction = glm::vec3(
                cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
            vertices.push_back({.position = radius * direction, .normal = direction});
        }
    }

    for (size_t ring = 0; ring < rings; ring++) {
        for (size_t segment = 0; segment < segments; segment++) {
            uint32_t v1 = ring * (segments + 1) + segment;
            uint32_t v2 = v1 + 1;
            uint32_t v3 = (ring + 1) * (segments + 1) + segment;
            uint32_t v4 = v3 + 1;
            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v3);

            indices.push_back(v4);
            indices.push_back(v2);
            indices.push_back(v3);

            // indices.push_back(ring * (segments + 1) + segment);
            // indices.push_back((ring + 1) * (segments + 1) + segment);
            // indices.push_back((ring + 1) * (segments + 1) + segment + 1);

            // indices.push_back(ring * (segments + 1) + segment);
            // indices.push_back((ring + 1) * (segments + 1) + segment + 1);
            // indices.push_back(ring * (segments + 1) + segment + 1);
        }
    }

    return Mesh(vertices, indices);
}
