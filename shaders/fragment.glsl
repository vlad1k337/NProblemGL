#version 410 core

in vec3 frag_pos;
out vec4 out_color;

void main()
{
    out_color = vec4(abs(sin(frag_pos)) - 0.2, 1.0);
}
