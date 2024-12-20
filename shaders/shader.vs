#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 inColor;

uniform mat4 gTransform;

out vec4 color;

void main() {
    gl_Position = gTransform * vec4(position, 1.0);
    color = inColor;
}