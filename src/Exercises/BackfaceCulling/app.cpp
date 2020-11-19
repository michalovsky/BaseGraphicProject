//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <tuple>
#include <vector>

#include "Application/utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


void SimpleShapeApplication::init()
{

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program)
    {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"
                  << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"
                  << " shader files" << std::endl;
    }

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    glm::mat4 projection =  glm::perspective(
        glm::radians(45.0f),
        650.0f / 480.0f,
        1.0f,
        150.0f
    );
    glm::mat4 view = glm::lookAt(
        glm::vec3(2.5f, 2.0f, 5.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 model(1.0f);
    glm::mat4 pvm = projection * view * model;


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



    GLuint idx_buffer_handle;
    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint u_buffer_handle[2];
    glGenBuffers(2, &u_buffer_handle[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[0]);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    float lightIntensity = 1;
    float lightColor[3] = {1.0, 1.0, 1.0};
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &lightIntensity);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), lightColor);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_buffer_handle[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &pvm[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_buffer_handle[1]);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
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

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame()
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}
