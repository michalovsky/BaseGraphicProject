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
    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    if (!program)
    {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"
                  << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"
                  << " shader files" << std::endl;
    }

    auto u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if (u_diffuse_map_location == -1)
    {
        std::cerr << "Cannot find uniform diffuse_map\n";
    }
    else
    {
        glUniform1ui(u_diffuse_map_location, 0);
    }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }
    auto u_light_index = glGetUniformBlockIndex(program, "Light");
    if (u_light_index == -1)
    {
        std::cerr << "Cannot find uniform Light" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_light_index, 2);
    }

    quad = std::make_unique<Quad>();
    camera = std::make_unique<Camera>();
    cameraController = std::make_unique<CameraController>(camera.get());

    this->set_controller(cameraController.get());
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    this->getCamera()->setProjection(glm::pi<float>() / 4.0, (float)w / h, 0.1f, 100.0f);
    this->getCamera()->lookAt(glm::vec3(-2.5f, 2.0f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));

    glGenBuffers(1, &u_pvm_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    auto V = this->getCamera()->getView();

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->getCamera()->getProjection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &V[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &this->u_light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_light_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    this->light.position = V * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    this->light.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->light.a = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
    this->light.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &this->light.position[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &this->light.color[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &this->light.a[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &this->light.ambient[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, this->u_light_buffer);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
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
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_pvm_buffer);
    auto P = this->getCamera()->getProjection();
    auto VM = this->getCamera()->getView();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3),
                    &N[2]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    this->light.position = VM * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &this->light.position[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    quad->draw();
}
