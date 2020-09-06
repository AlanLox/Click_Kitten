#version 430 core

layout (location = 0) in float angle;
layout (location = 1) uniform vec2 ab;
layout (location = 2) uniform vec2 center;
layout (location = 3) uniform vec3 color;
layout (location = 8) uniform float offset;

out vec4 Color;

void main()
{
    gl_Position = vec4(center.x + ab.x * cos(angle), center.y + ab.y * sin(angle) + offset , 0.0, 1.0);
    Color = vec4(color, 1.0f);
}   