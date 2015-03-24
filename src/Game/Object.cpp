#include "Object.hpp"

Object::Object(const uint32_t id) :
    _id(id)
{}

/*
glGenBuffers(1, &ibo_cube_elements);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
*/