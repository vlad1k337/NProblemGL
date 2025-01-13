#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 offsets;

precision highp float;

uniform mat4 view;
uniform mat4 proj;

out vec3 frag_pos;

void main()
{
    frag_pos = offsets;
    gl_Position = proj * view * vec4(position + offsets, 1.0);
}
