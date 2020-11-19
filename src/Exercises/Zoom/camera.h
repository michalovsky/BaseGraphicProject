#pragma once
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
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

    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
    {
        view = glm::lookAt(eye, center, up);
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

    glm::mat4 getView() const
    {
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
    glm::mat4 view;
};