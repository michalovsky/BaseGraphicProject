#pragma once

#include "camera.h"

class CameraController
{
public:
    CameraController() : camera(nullptr), scale{0.01}, leftMouseButtonPressed{false} {}
    CameraController(Camera* cameraInit) : camera(cameraInit), scale{0.01}, leftMouseButtonPressed{false} {}

    void set_camera(Camera* cameraInit)
    {
        camera = cameraInit;
    }

    void rotate_camera(float dx, float dy)
    {
        camera->rotate_around_center(-scale * dy, camera->getX());
        camera->rotate_around_center(-scale * dx, glm::vec3{0.0f, 0.0f, 1.0f});
    }
    void mouse_moved(float x, float y)
    {
        if (leftMouseButtonPressed)
        {
            auto dx = x - mousePositionX;
            auto dy = y - mousePositionY;
            mousePositionX = x;
            mousePositionY = y;

            rotate_camera(dx, dy);
        }
    };
    void onLeftMouseButtonPressed(float x, float y)
    {
        leftMouseButtonPressed = true;
        mousePositionX = x;
        mousePositionY = y;
    };

    void onLeftMouseButtonReleased(float x, float y)
    {
        leftMouseButtonPressed = false;
        auto dx = x - mousePositionX;
        auto dy = y - mousePositionY;

        rotate_camera(dx, dy);
    };

private:
    Camera* camera;
    float scale;
    bool leftMouseButtonPressed;
    float mousePositionX;
    float mousePositionY;
};