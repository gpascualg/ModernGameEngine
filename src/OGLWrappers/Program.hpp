#pragma once

#include "core.hpp"

class Program
{
public:
    Program();

    inline GLuint operator*()
    {
        return this->_program;
    }

    bool attach(GLenum type, std::string path);
    bool link();

private:
    GLuint compile(GLenum type, std::string path);

private:
    GLuint _program;
};
