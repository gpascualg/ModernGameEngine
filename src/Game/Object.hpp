#pragma once

#include "core.hpp"

class Object
{
public:
    Object(const uint32_t id);
    virtual ~Object();

    virtual void initialize() = 0;
    virtual void draw() = 0;

    LFS_INLINE uint32_t getID();

private:
    uint32_t _id;
};

uint32_t Object::getID()
{
    return _id;
}
