#ifndef PRIMITIVES_H
#define PRIMITIVES_H

// clang-format off
#include "mesh.hpp"
#include <cstdint>

const float triangle[] = {
    -0.5f,  0.0f, -0.5f, 1.f, 0.f, 0.f,
     0.5f,  0.0f, -0.5f, 0.f, 1.f, 0.f,
     0.0f,  0.0f,  0.5f, 0.f, 0.f, 1.f
};
// clang-format on

Mesh generate_sphere(uint32_t rings, uint32_t segments, float radius);

#endif  // PRIMITIVES_H
