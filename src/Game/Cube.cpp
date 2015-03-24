#include "Cube.hpp"

#include <glm/gtc/matrix_transform.hpp>

Cube::Cube(const uint32_t id) :
    Object(id)
{
    // Vertices of a unit cube centered at origin, sides aligned with axes
    vertices[0] = point4( -0.5, -0.5,  0.5, 1.0 );
    vertices[1] = point4( -0.5,  0.5,  0.5, 1.0 );
    vertices[2] = point4(  0.5,  0.5,  0.5, 1.0 );
    vertices[3] = point4(  0.5, -0.5,  0.5, 1.0 );
    vertices[4] = point4( -0.5, -0.5, -0.5, 1.0 );
    vertices[5] = point4( -0.5,  0.5, -0.5, 1.0 );
    vertices[6] = point4(  0.5,  0.5, -0.5, 1.0 );
    vertices[7] = point4(  0.5, -0.5, -0.5, 1.0 );

    // RGBA colors
    vertex_colors[0] =    color4( 0.0, 0.0, 0.0, 1.0 );  // black
    vertex_colors[1] =    color4( 1.0, 0.0, 0.0, 1.0 );  // red
    vertex_colors[2] =    color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    vertex_colors[3] =    color4( 0.0, 1.0, 0.0, 1.0 );  // green
    vertex_colors[4] =    color4( 0.0, 0.0, 1.0, 1.0 );  // blue
    vertex_colors[5] =    color4( 1.0, 0.0, 1.0, 1.0 );  // magenta
    vertex_colors[6] =    color4( 1.0, 1.0, 1.0, 1.0 );  // white
    vertex_colors[7] =    color4( 0.0, 1.0, 1.0, 1.0 );  // cyan
}

void Cube::quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

void Cube::initialize()
{
    Index = 0;
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );

    glGenBuffers(1, &_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), &colors[0]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, _buffers);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(points));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::rotate(glm::vec3 rotation, GLfloat angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), angle, rotation);
    for (int i = 0; i < NumVertices; ++i)
    {
        points[i] = rotationMatrix * points[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, _buffers);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), &points[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
