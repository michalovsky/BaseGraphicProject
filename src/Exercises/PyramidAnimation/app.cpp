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
        glm::vec3(-2.5f, 2.0f, -2.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );


    glGenBuffers(1, &u_pvm_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
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
    auto rotation_angle = glm::two_pi<float>()*elapsed_time/rotationPeriod;

    glm::mat4 main_TR = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

    glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation_angle ,glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotated = camera->getView() * R;

    auto orbital_rotation_angle = 2.0f * glm::pi<float>() * elapsed_time / orbitalRotationPeriod;
    auto x = 10 * cos(orbital_rotation_angle);
    auto z = 8 * sin(orbital_rotation_angle);
    glm::mat4 O = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z));
    auto orbited = camera->getView() * O * R;

    auto moon_rotation_period = 10.0f;
    auto moon_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/moon_rotation_period;
    float x_m = 3.0f*cos(moon_rotation_angle);
    float y_m = 3.0f*sin(moon_rotation_angle);
    glm::mat4 O_moon = glm::translate(glm::mat4(1.0f), glm::vec3(x_m,y_m,0));
    glm::mat4 R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.794f, 0.794f, 0.794f));

    glm::mat4 moon_transform = camera->getView() * O * O_moon * R_moon * S;

    auto orbited2 = camera->getView() * O;
    auto satellite_rotation_period = 2.0f;
    auto satellite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satellite_rotation_period;
    float x_sat = 1.5*cos(satellite_rotation_angle);
    float z_sat = 1.5*sin(satellite_rotation_angle);
    glm::mat4 O_sat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, x_sat, z_sat));
    glm::mat4 R_sat = glm::rotate(glm::mat4(1.0f), satellite_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 S_sat = glm::scale(glm::mat4(1.0f), glm::vec3(0.63f, 0.63f, 0.63f));
    glm::mat4 sat_transform = orbited2 * O_sat * R_sat * S_sat;


    glm::mat4 transforms[] = {
        rotated,
        orbited,
        moon_transform,
        sat_transform
    };

    for (glm::mat4 transform : transforms)
    {
        auto moved_to_main_pyramid_center = transform * main_TR;
        glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer);
        auto PVM = getCamera()->getProjection() * getCamera()->getView() * transform * main_TR;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &moved_to_main_pyramid_center[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        this->pyramid->draw();
    }

}


