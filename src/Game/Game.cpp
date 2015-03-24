
#include "Game.hpp"

#include "Cube.hpp"
#include "Object.hpp"
#include "Program.hpp"
#include "Resources.hpp"
#include "Window.hpp"


Game::Game(Window* window) :
    Updater(),
    _window(window),
    xRot(0), yRot(0), zRot(0),
    lastX(0), lastY(0)
{
    _program = new Shader::Program();
    _cube = new Cube(0);
    _cube->initialize();
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

    /* Update (UNIX won't render unless called, windows does) */
    _window->update();
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

    if (mouse)
    {
        ((Cube*)_cube)->rotate({1.0f, 0.0f, 0.0f}, xRot / 1800.0f);
        ((Cube*)_cube)->rotate({0.0f, 0.0f, 1.0f}, zRot / 1800.0f);
        _window->update();
    }

    lastX = x;
    lastY = y;
}

void Game::draw()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _cube->draw();
}
