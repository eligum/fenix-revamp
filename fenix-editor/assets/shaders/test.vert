#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_texcoord;

out vec3 v_color;
out vec3 v_texcoord;

uniform mat4 u_projection_view;
uniform mat4 u_transform;

void main()
{
    v_color = a_color;
    v_texcoord = a_texcoord;

    gl_Position = u_projection_view * u_transform * vec4(a_position, 1.0);
}
