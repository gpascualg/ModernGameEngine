#pragma once

#include "core.hpp"
#include "Object.hpp"


const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

class Cube : public Object
{
public:
    Cube(const uint32_t id);

    void quad( int a, int b, int c, int d );
    void initialize() override;
    void draw() override;
    void rotate(glm::vec3 rotation, GLfloat angle);

private:
    GLuint _buffers;


    point4 vertices[8]; // 8 vertexs del cub
    color4 vertex_colors[8]; // 8 colors RGBA associats a cada vertex

    int Index;
    point4 points[NumVertices]; // 36 punts: cub triangulat
    color4 colors[NumVertices]; // colors en aquests punts
};
