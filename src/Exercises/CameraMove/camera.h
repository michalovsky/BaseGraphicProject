#pragma once
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "rotation.h"

class Camera
{
public:
    void rotate_around_center(float angle, const glm::vec3 &axis)
    {
        rotate_around_point(angle, axis, center);
    }

    void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c)
    {
        auto R = rotation(angle, axis);
        xOrientation = R * xOrientation;
        yOrientation = R * yOrientation;
        zOrientation = R * zOrientation;

        auto t = cameraPosition - c;
        t = R * t;
        cameraPosition = c + t;
    }

    void zoom(float mouseRollValue)
    {
        auto x = fovy / glm::pi<float>();
        auto y = inverseLogistic(x);
        y += mouseRollValue;
        x = logistic(y);
        fovy = x * glm::pi<float>();
    }

    float logistic(float y)
    {
        return 1.0f / (1.0f + std::exp(-y));
    }

    float inverseLogistic(float x)
    {
        return std::log(x / (1.0f - x));
    }

    void lookAt(const glm::vec3& eyeInit, const glm::vec3& centerInit, const glm::vec3& upInit)
    {
        zOrientation = glm::normalize(eyeInit - centerInit);
        xOrientation = glm::normalize(glm::cross(upInit, zOrientation));
        yOrientation = glm::normalize(glm::cross(zOrientation, xOrientation));

        cameraPosition = eyeInit;
        center = centerInit;
    }

    void setProjection(float fovyInit, float aspectInit, float zNearInit, float zFarInit)
    {
        fovy = fovyInit;
        aspect = aspectInit;
        zNear = zNearInit;
        zFar = zFarInit;
    }

    void setAspect(float aspectInit)
    {
        aspect = aspectInit;
    }

    glm::vec3 getX() const
    {
        return xOrientation;
    }

    glm::vec3 getY() const
    {
        return yOrientation;
    }

    glm::vec3 getZ() const
    {
        return zOrientation;
    }

    glm::vec3 getPosition() const
    {
        return cameraPosition;
    }

    glm::vec3 getCenter() const
    {
        return center;
    }

    glm::mat4 getView() const
    {
        glm::mat4 view(1.0f);
        for (int i = 0; i < 3; ++i)
        {
            view[i][0] = xOrientation[i];
            view[i][1] = yOrientation[i];
            view[i][2] = zOrientation[i];
        }

        auto t = -glm::vec3{
            glm::dot(xOrientation, cameraPosition),
            glm::dot(yOrientation, cameraPosition),
            glm::dot(zOrientation, cameraPosition),
        };
        view[3] = glm::vec4(t, 1.0f);

        return view;
    }

    glm::mat4 getProjection() const
    {
        return glm::perspective(fovy, aspect, zNear, zFar);
    }

private:
    float fovy;
    float aspect;
    float zNear;
    float zFar;
    glm::vec3 cameraPosition;
    glm::vec3 center;
    glm::vec3 xOrientation;
    glm::vec3 yOrientation;
    glm::vec3 zOrientation;
};