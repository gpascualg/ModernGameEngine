#pragma once

#include "core.hpp"

namespace Shader
{
    class Program;
}

class Object
{
public:
    Object(const uint32_t id);

    virtual void initialize(Shader::Program* program) = 0;
    virtual void draw() = 0;

private:
    uint32_t _id;
};
