//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <vector>

#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h"
#include "camera_controller.h"
#include "glad/glad.h"
#include "quad.h"

struct Light {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec4 ambient;
};

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

    void set_camera(Camera* cameraInit)
    {
        camera = std::make_unique<Camera>(*cameraInit);
    }

    void set_controller(CameraController* cameraControllerInit)
    {
        cameraController = std::make_unique<CameraController>(*cameraControllerInit);
    }

    Camera* getCamera()
    {
        return camera.get();
    }

    void scroll_callback(double xoffset, double yoffset) override;
    void mouse_button_callback(int button, int action, int mods) override;
    void cursor_position_callback(double x, double y) override;

private:
    std::unique_ptr<Quad> quad;
    GLuint u_pvm_buffer;
    GLuint u_light_buffer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraController> cameraController;
    std::chrono::steady_clock::time_point start;
    float rotationPeriod = 4.0f;
    float orbitalRotationPeriod = 20.0f;
    Light light;
};