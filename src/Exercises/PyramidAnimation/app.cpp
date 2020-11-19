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
    camera->lookAt(
        glm::vec3(2.5f, 50.0f, 5.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    model = glm::mat4(1.0f);


//    GLuint u_light_buffer;
//    glGenBuffers(1, &u_light_buffer);
//    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer);
//    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
//    float lightIntensity = 1;
//    float lightColor[3] = {1.0, 1.0, 1.0};
//    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &lightIntensity);
//    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), lightColor);
//    glBindBuffer(GL_UNIFORM_BUFFER, 0);
//    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_light_buffer);


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
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();
    pyramid->draw();

    auto orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / orbitalRotationPeriod;
    auto x = 1 * cos(orbital_rotation_angle);
    auto z = 1 * sin(orbital_rotation_angle);
    model = glm::translate(model, glm::vec3(x, 0, z));
//    auto rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / rotationPeriod;

    model = glm::rotate(model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f));


    glm::mat4 pvm = camera->getProjection() * camera->getView() * model;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &pvm[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


