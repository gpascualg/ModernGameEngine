#include "Game.h"
#include "Window.h"

Game::Game(Window* window) :
    Updater(),
    _window(window),
    xRot(0), yRot(0), zRot(0),
    lastX(0), lastY(0)
{}

int Game::update()
{
    return Updater::update();
}

void Game::initializeGL()
{
    /* Enable some gl constants */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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

    glPushMatrix();

    /*      Posa el color a vermell */
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(-0.5, -0.5, -0.5);

    /*      Crea un cub de  a x h x p */
    glScalef((GLfloat)1, (GLfloat)1, (GLfloat)1);
    glPopMatrix();
    glPushMatrix();
    glBegin(GL_QUADS);		// Draw The Cube Using quads
        glColor3f(0.0f, 1.0f, 0.0f);	// Color Blue
        glVertex3f(1.0f, 1.0f, -1.0f);	// Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, -1.0f);	// Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
        glVertex3f(1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
        glColor3f(1.0f, 0.5f, 0.0f);	// Color Orange
        glVertex3f(1.0f, -1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f, -1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f, -1.0f, -1.0f);// Bottom Left Of The Quad (Bottom)
        glVertex3f(1.0f, -1.0f, -1.0f);	// Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f, 0.0f, 0.0f);	// Color Red
        glVertex3f(1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
        glVertex3f(-1.0f, -1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
        glVertex3f(1.0f, -1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
        glColor3f(1.0f, 1.0f, 0.0f);	// Color Yellow
        glVertex3f(1.0f, -1.0f, -1.0f);	// Top Right Of The Quad (Back)
        glVertex3f(-1.0f, -1.0f, -1.0f);// Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f, -1.0f);	// Bottom Left Of The Quad (Back)
        glVertex3f(1.0f, 1.0f, -1.0f);	// Bottom Right Of The Quad (Back)
        glColor3f(0.0f, 0.0f, 1.0f);	// Color Blue
        glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f, -1.0f);	// Top Left Of The Quad (Left)
        glVertex3f(-1.0f, -1.0f, -1.0f);// Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f, -1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
        glColor3f(1.0f, 0.0f, 1.0f);	// Color Violet
        glVertex3f(1.0f, 1.0f, -1.0f);	// Top Right Of The Quad (Right)
        glVertex3f(1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
        glVertex3f(1.0f, -1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
        glVertex3f(1.0f, -1.0f, -1.0f);	// Bottom Right Of The Quad (Right)
    glEnd();			// End Drawing The Cube
    glPopMatrix();
}
