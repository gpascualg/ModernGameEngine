#include "Game.hpp"

#include "Object.hpp"
#include "Program.hpp"
#include "Resources.hpp"
#include "Window.hpp"


GLfloat colors[6][4] = {
    { 0.0f, 1.0f, 0.0f, 1.0f },	// Color Blue
    { 1.0f, 0.5f, 0.0f, 1.0f },	// Color Orange
    { 1.0f, 0.0f, 0.0f, 1.0f },	// Color Red
    { 1.0f, 1.0f, 0.0f, 1.0f },	// Color Yellow
    { 0.0f, 0.0f, 1.0f, 1.0f },	// Color Blue
    { 1.0f, 0.0f, 1.0f, 1.0f }	// Color Violet
};

GLfloat points[4 * 6][4] = {
    { 1.0f, 1.0f, -1.0f, 1.0f },
    { -1.0f, 1.0f, -1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },

    { 1.0f, -1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f, 1.0f },

    { 1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f, 1.0f, 1.0f },

    { 1.0f, -1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f, 1.0f },
    { -1.0f, 1.0f, -1.0f, 1.0f },
    { 1.0f, 1.0f, -1.0f, 1.0f },

    { -1.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f, 1.0f },

    { 1.0f, 1.0f, -1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f, 1.0f },
};

class Cube : public Object
{
public:
    Cube(const uint32_t id) :
        Object(id)
    {}

    void initialize(Shader::Program* program) override
    {
        _program = program;

        glGenBuffers(1, &_buffers);
        glBindBuffer(GL_ARRAY_BUFFER, _buffers);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        
        glEnable(GL_DEPTH_TEST);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw() override
    {
        glBindBuffer(GL_ARRAY_BUFFER, _buffers);
        
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(4, GL_FLOAT, 0, 0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat) * 4 * 4 * 6));
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_QUADS, 0, 4*6);

        glDisableClientState(GL_VERTEX_ARRAY);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    Shader::Program* _program;
    GLuint _buffers;
};

Game::Game(Window* window) :
    Updater(),
    _window(window),
    xRot(0), yRot(0), zRot(0),
    lastX(0), lastY(0)
{
    _program = new Shader::Program();
    _cube = new Cube(0);
    _cube->initialize(_program);
}

int Game::update()
{
    return Updater::update();
}

void Game::initializeGL()
{
    /* Enable some gl constants */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /* Create shaders */
    _program->attach(GL_VERTEX_SHADER, Resources::getPath("shaders", "vertex.glsl"));
    _program->attach(GL_FRAGMENT_SHADER, Resources::getPath("shaders", "fragment.glsl"));

    glBindAttribLocation(**_program, 0, "vPosition");
    glBindAttribLocation(**_program, 1, "vColor");

    _program->link();
    _program->bind();

    /* Clear color */
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::onResize(int width, int height)
{
    int side = std::min(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#else
    glOrtho(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void Game::onMouseMove(double x, double y, uint8_t mouse)
{
    double dx = x - lastX;
    double dy = y - lastY;

    if (mouse & Mouse::LeftButton) {
        xRot += dy;
    }
    else if (mouse & Mouse::RightButton) {
        xRot += dy;
        zRot += dx;
    }

    while (xRot > 360)
        xRot -= 180;
    while (zRot > 360)
        zRot -= 180;

    lastX = x;
    lastY = y;

    _window->update();
}

void Game::draw()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef((GLfloat)xRot, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)yRot, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)zRot, 0.0, 0.0, 1.0);

    _cube->draw();
}
