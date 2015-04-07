#pragma once

#include "core.hpp"
#include "Object.hpp"

namespace Shader
{
    class Program;
}

const int NumVertices = 36;

class Cube : public Object
{
public:
    Cube(const uint32_t id, Shader::Program* program);

    void quad( int a, int b, int c, int d );
    void initialize() override;
    void draw() override;
    void rotate(glm::vec3 rotation, GLfloat angle);

private:
    GLuint _buffers;
    Shader::Program* _program;

    point4 vertices[8]; // 8 vertexs del cub
    color4 vertex_colors[8]; // 8 colors RGBA associats a cada vertex

    int Index;
    point4 points[NumVertices]; // 36 punts: cub triangulat
    color4 colors[NumVertices]; // colors en aquests punts
};
