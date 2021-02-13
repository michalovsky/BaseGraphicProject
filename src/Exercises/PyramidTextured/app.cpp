//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <tuple>
#include <vector>

#include "Application/utils.h"

void SimpleShapeApplication::init()
{
    pyramid = std::make_unique<Pyramid>();
    camera = std::make_unique<Camera>();
    cameraController = std::make_unique<CameraController>(camera.get());

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program)
    {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"
                  << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"
                  << " shader files" << std::endl;
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

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    auto fovy = glm::radians(45.0f);
    auto aspect = static_cast<float>(w) / static_cast<float>(h);
    auto zNear = 0.1f;
    auto zFar = 100.0f;
    camera->setProjection(fovy, aspect, zNear, zFar);
    camera->lookAt(glm::vec3(2.5f, 2.0f, 5.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glGenBuffers(1, &u_pvm_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    start = std::chrono::steady_clock::now();

    auto u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if (u_diffuse_map_location == -1)
    {
        std::cerr << "Cannot find uniform diffuse_map\n";
    }
    else
    {
        glUniform1ui(u_diffuse_map_location, 0);
    }
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h)
{
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera->setAspect(static_cast<float>(w) / static_cast<float>(h));
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset)
{
    Application::scroll_callback(xoffset, yoffset);
    camera->zoom(static_cast<float>(yoffset) / 30.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods)
{
    Application::mouse_button_callback(button, action, mods);

    if (cameraController)
    {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            cameraController->onLeftMouseButtonPressed(x, y);
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            cameraController->onLeftMouseButtonReleased(x, y);
        }
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y)
{
    Application::cursor_position_callback(x, y);
    if (cameraController)
    {
        cameraController->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::frame()
{
    auto PVM = camera->getProjection() * camera->getView() * glm::mat4(1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    pyramid->draw();
}
