#pragma once

#include "core.hpp"
#include "Scheduler.hpp"

#include <glm/gtc/matrix_transform.hpp>

class Object
{
public:
    Object(const uint32_t id);
    virtual ~Object();

    virtual void initialize(void*) = 0;
    virtual void draw(float interpolate) = 0;
    
    virtual int updateCPU(void* arg0) = 0;
    virtual void updateGPU() = 0;
	
    LFS_INLINE uint32_t getID();
	LFS_INLINE glm::vec3 getPosition();

    virtual LFS_INLINE void rotate(glm::vec3 rotation, GLfloat angle);
	virtual LFS_INLINE void translate(glm::vec3 trans);

signals:
	SIGNAL(moved());
	SIGNAL(rotated(glm::vec3 rotation, GLfloat angle));

protected:
    point4* _points;
	color4* _colors;
	glm::vec3 _position;

    glm::mat4 _transformations;


private:
    uint32_t _id;
};

uint32_t Object::getID()
{
    return _id;
}

LFS_INLINE glm::vec3 Object::getPosition()
{
	return _position;
}

void Object::rotate(glm::vec3 rotation, GLfloat angle) {
	_transformations = glm::rotate(glm::mat4(), angle, rotation) * _transformations;
}

void Object::translate(glm::vec3 trans)
{
	_transformations = glm::translate(glm::mat4(), trans) * _transformations;
}