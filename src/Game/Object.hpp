#pragma once

#include "core.hpp"

class Object
{
public:
    Object(const uint32_t id);

    virtual void initialize() = 0;
    virtual void draw() = 0;

private:
    uint32_t _id;
};
