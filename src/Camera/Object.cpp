#include "Object.hpp"

Object::Object(const uint32_t id) :
    _id(id),
    _points(nullptr),
    _colors(nullptr),
	_position(0, 0, 0)
{
}

Object::~Object()
{}
