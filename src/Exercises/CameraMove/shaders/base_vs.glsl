#version 410

layout(location=0) in vec4 aVertexPosition;
layout(location=1) in vec3 aVertexColor;

layout(std140) uniform Transformations
{
    mat4 PVM;
};

out vec3 vertex_color;

void main()
{
    vertex_color = aVertexColor;
    gl_Position = PVM * aVertexPosition;
}
