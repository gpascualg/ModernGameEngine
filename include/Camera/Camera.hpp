#pragma once

#include "core.hpp"
#include "Program.hpp"
#include "Object.hpp"
#include <glm/gtc/constants.hpp>


class Camera
{
public:
    Camera(glm::vec3 sceneDimensions);
	
    LFS_INLINE void rotateCamera(double angX, double angY) // 2D coordinates -> 3D coordinates
    {
		angX = _angX - angX;
		angY = _angY - angY;

        glm::vec3 dir = glm::vec3(cos(angY) * sin(angX), sin(angY), cos(angY) * cos(angX));
		glm::vec3 right = glm::vec3(sin(angX - glm::half_pi<double>()), 0, cos(angX - glm::half_pi<double>()));
		glm::vec3 vup = glm::cross(right, dir);

		if (!_interpolate)
		{
			_angX = angX;
			_angY = angY;

			_dir = dir;
			_right = right;
			_vup = vup;
			_front = glm::vec3(sin(_angX), 0, cos(_angX));
		}

        calculateViewMatrix();
    }

    LFS_INLINE void moveCamera(glm::vec3 speed, glm::vec3 axis)
    {
		glm::vec3 obs = _obs + speed * (axis[0] * (_locked ? _front : _dir) + axis[1] * _vup + axis[2] * _right);

		if (!_interpolate)
		{
			_obs = obs;
		}

        calculateViewMatrix();
    }

	LFS_INLINE void setInterpolation(bool interpolate)
	{
		_interpolate = interpolate;
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

    LFS_INLINE void setLocked(bool locked)
    {
        _locked = locked;
    }

    LFS_INLINE bool getLocked()
    {
        return _locked;
    }

    void calculateViewMatrix();
    void calculateProjectionMatrix();

	void attachTo(Object* object);

	LFS_INLINE void onObjectMoved();
	LFS_INLINE void onObjectRotated(glm::vec3 rotation, GLfloat angle);

    LFS_INLINE glm::mat4 getMVP()
    {
        return _projection * _view * _model;
    }

    LFS_INLINE void toGPU(Shader::Program* program)
    {
		if (_needsGPUUpdate)
		{
			glUniformMatrix4fv(program->uniformLocation("MVP"), 1, GL_FALSE, &getMVP()[0][0]);
			_needsGPUUpdate = false;
		}
    }

private:
	Object* _attached;

	bool _needsGPUUpdate;
    bool _locked;
	bool _interpolate;

    float _aspect;

    double _angX;
    double _angY;

    glm::vec3 _front;
    glm::vec3 _right;

    glm::vec3 _obs;
    glm::vec3 _dir;
    glm::vec3 _vup;

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
};

void Camera::onObjectMoved()
{
	glm::vec4 result = _model * glm::vec4(_attached->getPosition(), 1);
	setOBS(glm::vec3(result.x, result.y, result.z) - _dir);
	//rotateCamera(0, 0);
}

void Camera::onObjectRotated(glm::vec3 rotation, GLfloat angle)
{
	rotateCamera(angle * rotation.y, angle * rotation.z);
}