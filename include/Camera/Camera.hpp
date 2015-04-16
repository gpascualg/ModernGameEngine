#pragma once

#include "core.hpp"
#include "Program.hpp"
#include <glm/gtc/constants.hpp>

class Camera
{
public:
    Camera(glm::vec3 sceneDimensions);

    LFS_INLINE void rotateCamera(double angX, double angY) // 2D coordinates -> 3D coordinates
    {
        _angX -= angX;
        _angY -= angY;

        _dir = glm::vec3(cos(_angY) * sin(_angX), sin(_angY), cos(_angY) * cos(_angX));
        _right = glm::vec3(sin(_angX - glm::half_pi<double>()), 0, cos(_angX - glm::half_pi<double>()));
        _vup = glm::cross(_right, _dir);

        calculateViewMatrix();
    }

    LFS_INLINE void moveCamera(glm::vec3 speed, glm::vec3 axis)
    {
        _obs = _obs + speed * (axis[0] * _dir + axis[1] * _vup + axis[2] * _right);

        calculateViewMatrix();
    }

    LFS_INLINE void setDIR(glm::vec3 dir)
    {
        _dir = dir;
        calculateViewMatrix();
    }

    LFS_INLINE void setOBS(glm::vec3 obs)
    {
        _obs = obs;
        calculateViewMatrix();
    }
    
    LFS_INLINE void setAspect(float aspect)
    {
        _aspect = aspect;
        calculateProjectionMatrix();
    }

    void calculateViewMatrix();
    void calculateProjectionMatrix();

    LFS_INLINE glm::mat4 getMVP()
    {
        return _projection * _view * _model;
    }

    LFS_INLINE void toGPU(Shader::Program* program)
    {
        glUniformMatrix4fv(program->uniformLocation("MVP"), 1, GL_FALSE, &getMVP()[0][0]);
    }

private:
    float _aspect;

    double _angX;
    double _angY;

    glm::vec3 _right;
    glm::vec3 _obs;
    glm::vec3 _dir;
    glm::vec3 _vup;

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};
