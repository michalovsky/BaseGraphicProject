#version 410

layout(location=0) in vec4 aVertexPosition;
layout(location=1) in vec2 aTextureCoords;

layout(std140) uniform Transformations
{
    mat4 PVM;
};

out vec2 texture_coords;

void main()
{
    texture_coords = aTextureCoords;
    gl_Position = PVM * aVertexPosition;
}
