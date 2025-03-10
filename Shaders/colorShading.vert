#version 330 core

layout (location = 0) in vec3 vertexPosition; // Vertex position
layout (location = 1) in vec2 vertexUV;      // Texture coordinates
layout (location = 2) in float normal;       // Light flag (1 if light, 0 if not)

out vec2 fragmentUV;
flat out int isLight;
out vec4 FragPosViewSpace; // Position in view space (needed for fog)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 chunkOffset;

void main() {
    vec4 worldPos = model * vec4(vertexPosition + chunkOffset, 1.0);
    gl_Position = projection * view * worldPos; // Final position

    FragPosViewSpace = view * worldPos; // Get view-space position for fog

    fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y); // Flip texture coordinates
    isLight = int(normal); // Store the light flag
}
