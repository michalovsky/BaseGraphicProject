#version 410

layout(location=0) out vec4 vFragColor;
in vec2 texture_coords;
uniform sampler2D diffuse_map;

void main()
{
    vFragColor = texture(diffuse_map, texture_coords);
}
