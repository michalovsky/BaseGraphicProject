#include "pyramid.h"
#include <vector>

Pyramid::Pyramid()
{
    std::vector<GLfloat> vertices = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 0.0, 0.0,

        0.5, 1.0, 0.5, 0.933, 0.73, 0.188,
        0.0, 0.0, 1.0, 0.933, 0.73, 0.188,
        1.0, 0.0, 1.0, 0.933, 0.73, 0.188,

        0.5, 1.0, 0.5, 0.455, 0.0, 0.004,
        0.0, 0.0, 0.0, 0.455, 0.0, 0.004,
        0.0, 0.0, 1.0, 0.455, 0.0, 0.004,

        0.5, 1.0, 0.5, 0.827, 0.651, 0.145,
        1.0, 0.0, 0.0, 0.827, 0.651, 0.145,
        0.0, 0.0, 0.0, 0.827, 0.651, 0.145,

        0.5, 1.0, 0.5, 0.682, 0.0, 0.004,
        1.0, 0.0, 1.0, 0.682, 0.0, 0.004,
        1.0, 0.0, 0.0, 0.682, 0.0, 0.004
    };

    std::vector<GLushort> indices = {0, 2, 1, 3, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};


    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(9 * sizeof(GLfloat)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(12 * sizeof(GLfloat)));
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(15 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Pyramid::~Pyramid() {

}

void Pyramid::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}