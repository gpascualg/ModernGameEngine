#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 sceneDimensions):
    _angX(0),
    _angY(0),
    _locked(false),
	_interpolate(false)
{
    _model = glm::scale(glm::mat4(), glm::vec3(2 / sceneDimensions[0], 2 / sceneDimensions[1], 2 / sceneDimensions[2]));
    _vup = glm::vec3(0, 1, 0);
    _dir = glm::vec3(-1, -1, 0); // _vrp = (0, 0, 0) = (1, 1, 0) + (-1, -1, 0)
    _obs = glm::vec3(1, 1, 0);

    _aspect = 4.0f/ 3.0f;

    calculateViewMatrix();
    calculateProjectionMatrix();
}

void Camera::calculateViewMatrix()
{
    _view = glm::lookAt(_obs, _obs + _dir, _vup);
	_needsGPUUpdate = true;
}

void Camera::calculateProjectionMatrix()
{
    _projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	_needsGPUUpdate = true;
}

void Camera::attachTo(Object* object)
{
	_attached = object;
	bind(object, &Object::moved, this, &Camera::onObjectMoved);
	bind(object, &Object::rotated, this, &Camera::onObjectRotated);
}

// 1 Obs, camera stans
// 1 VRP, View reference point (where we are looking to)
// 1 VUP, verticality (0, 1, 0)
