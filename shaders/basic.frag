#version 330 core
out vec4 FragColor;

in vec3 vNormal;

void main() {
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    FragColor = vec4(0.5*vNormal+0.5, 1.0);
}