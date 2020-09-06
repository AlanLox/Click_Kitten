#version 430 core

layout (location = 6) in vec2 position;
layout (location = 7) uniform vec3 color;
layout (location = 8) uniform float offset;

out vec4 Color;

void main()
{
    gl_Position = vec4(position.x, position.y + offset, 1.0, 1.0);
    Color = vec4(color, 1.0f);
}   