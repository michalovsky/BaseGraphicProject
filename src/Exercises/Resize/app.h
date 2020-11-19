//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Application/application.h"
#include "Application/utils.h"
#include "glad/glad.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1)
        : Application(width, height, title, major, minor)
    {
    }

    void init() override;
    void frame() override;
    void framebuffer_resize_callback(int w, int h) override;

private:
    GLuint vao_;
    GLuint u_pvm_buffer;
    float fovy;
    float aspect;
    float zNear;
    float zFar;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
};