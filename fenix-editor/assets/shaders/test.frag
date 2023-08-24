#version 450 core

in vec3 v_color;
in vec2 v_texcoord;

out vec4 frag_color;

uniform vec3 u_color;
// uniform sampler2D color_map1;
// uniform sampler2D color_map2;

void main()
{
    // frag_color = mix(
    //     texture(color_map1, v_texcoord),
    //     texture(color_map2, v_texcoord),
    //     0.25
    // );

    frag_color = vec4(u_color, 1.0);
}
