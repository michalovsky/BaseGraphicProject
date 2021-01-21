#include "piramid.h"

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"

#include <vector>
#include <string>
#include <iostream>

Pyramid::Pyramid()
{
    std::vector<GLfloat> vertices = {
        0.0f, 0.0f, 0.0f, 0.1910f, 0.5000f,
        0.0f, 0.0f, 1.0f, 0.5000f, 0.8090f,
        1.0f, 0.0f, 0.0f, 0.5000f, 0.1910f,
        1.0f, 0.0f, 1.0f, 0.8090f, 0.5000f,
        0.5f, 1.0f, 0.5f, 0.0f, 1.0f,
        0.5f, 1.0f, 0.5f, 1.0f, 1.0f,
        0.5f, 1.0f, 0.5f, 1.0f, 0.0f,
        0.5f, 1.0f, 0.5f, 0.0f, 0.0f

    };

    std::vector<GLushort> indices = {
        0,2,1,
        3,1,2,
        4,0,1,
        5,1,3,
        6,3,2,
        7,2,0
    };


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";
    int width, height, n_channels;
    uint8_t* data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);
    if (data)
    {
        std::cout << width << std::endl;
        std::cout << height << std::endl;
    }
    else
    {
        std::cout << "Cannot load file: " << texture_filename << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Pyramid::draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}