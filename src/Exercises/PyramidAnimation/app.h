//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <vector>
#include <chrono>

#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h"
#include "camera_controller.h"
#include "glad/glad.h"
#include "pyramid.h"

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
    std::unique_ptr<Pyramid> pyramid;
    GLuint u_pvm_buffer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<CameraController> cameraController;
    std::chrono::steady_clock::time_point start;
    float rotationPeriod = 4.0f;
    float orbitalRotationPeriod = 20.0f;
};