#pragma once

#include "glad/glad.h"

class Quad
{
public:
    Quad();
    void draw();

private:
    GLuint vao;
    GLuint idx_buffer_handle;
    GLuint v_buffer_handle;
    GLuint diffuse_texture;
};