//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <memory>

#include "Application/application.h"
#include "Application/utils.h"
#include "camera.h"
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
    void set_camera(Camera *cameraInit) { camera = std::make_unique<Camera>(*cameraInit); }
    Camera* getCamera() { return camera.get(); }
    void scroll_callback(double xoffset, double yoffset) override;

private:
    GLuint vao_;
    GLuint u_pvm_buffer;
    std::unique_ptr<Camera> camera;
    glm::mat4 model;
};