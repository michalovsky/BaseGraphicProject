#pragma once

#include "glad/glad.h"

class Pyramid
{
public:
    Pyramid();
    void draw();

private:
    GLuint vao;
    GLuint idx_buffer_handle;
    GLuint v_buffer_handle;
    GLuint diffuse_texture;
};