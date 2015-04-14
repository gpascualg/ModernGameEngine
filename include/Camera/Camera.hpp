#pragma once

#include "core.hpp"

class Camera
{
public:
	Camera(glm::vec3 sceneDimensions);

	LFS_INLINE void setVRP(glm::vec3 vrp)
	{
		_vrp = vrp;
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

private:
	float _aspect;

	glm::vec3 _obs;
	glm::vec3 _vrp;
	glm::vec3 _vup;

	glm::mat4 _model;
	glm::mat4 _view;
	glm::mat4 _projection;
};
