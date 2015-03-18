#include "Window.h"
#include "Scheduler.h"

std::map<uintptr_t, Window*> Window::_windowToThis;

Window::Window(uint32_t width, uint32_t height, const char* title):
    _window(nullptr),
    _update(true)
{
    static bool libraryInitialized = false;
    if (!libraryInitialized)
    {
        if (!glfwInit())
            return;
    }

    _window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return;
    }

    /* Save current window */
    _windowToThis[(uintptr_t)_window] = this;

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    /* Setup callbacks */
    glfwSetWindowSizeCallback(_window, _resizeHandler);

    /* Enable some gl constants */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Window::mainloop(Scheduler* scheduler)
{
    if (!_window)
    {
        return;
    }

    // Emit resize
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    Broadcast<int, int>::emit(this, &Window::resize, width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(_window))
    {
        /* Update scheduler */
        scheduler->update();

        if (1)
        {
            _update = false;

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
            glTranslatef(0.0, 0.0, -10.0);
            glRotatef(80 / 16.0, 1.0, 0.0, 0.0);
            glRotatef(200 / 16.0, 0.0, 1.0, 0.0);
            glRotatef(0 / 16.0, 0.0, 0.0, 1.0);

            glPushMatrix();

            /*      Posa el color a vermell */
            glColor3f(1.0f, 0.0f, 0.0f);
            glTranslatef(-0.5, -0.5, -0.5);

            /*      Crea un cub de  a x h x p */
            glScalef((GLfloat)1, (GLfloat)1, (GLfloat)1);
            glPopMatrix();
            glPushMatrix();
            glBegin(GL_QUADS);		// Draw The Cube Using quads
                glColor3f(0.0f,1.0f,0.0f);	// Color Blue
                glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Top)
                glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Top)
                glVertex3f(-1.0f, 1.0f, 1.0f);	// Bottom Left Of The Quad (Top)
                glVertex3f( 1.0f, 1.0f, 1.0f);	// Bottom Right Of The Quad (Top)
                glColor3f(1.0f,0.5f,0.0f);	// Color Orange
                glVertex3f( 1.0f,-1.0f, 1.0f);	// Top Right Of The Quad (Bottom)
                glVertex3f(-1.0f,-1.0f, 1.0f);	// Top Left Of The Quad (Bottom)
                glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Bottom)
                glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Bottom)
                glColor3f(1.0f,0.0f,0.0f);	// Color Red
                glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Front)
                glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Front)
                glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Front)
                glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Front)
                glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
                glVertex3f( 1.0f,-1.0f,-1.0f);	// Top Right Of The Quad (Back)
                glVertex3f(-1.0f,-1.0f,-1.0f);	// Top Left Of The Quad (Back)
                glVertex3f(-1.0f, 1.0f,-1.0f);	// Bottom Left Of The Quad (Back)
                glVertex3f( 1.0f, 1.0f,-1.0f);	// Bottom Right Of The Quad (Back)
                glColor3f(0.0f,0.0f,1.0f);	// Color Blue
                glVertex3f(-1.0f, 1.0f, 1.0f);	// Top Right Of The Quad (Left)
                glVertex3f(-1.0f, 1.0f,-1.0f);	// Top Left Of The Quad (Left)
                glVertex3f(-1.0f,-1.0f,-1.0f);	// Bottom Left Of The Quad (Left)
                glVertex3f(-1.0f,-1.0f, 1.0f);	// Bottom Right Of The Quad (Left)
                glColor3f(1.0f,0.0f,1.0f);	// Color Violet
                glVertex3f( 1.0f, 1.0f,-1.0f);	// Top Right Of The Quad (Right)
                glVertex3f( 1.0f, 1.0f, 1.0f);	// Top Left Of The Quad (Right)
                glVertex3f( 1.0f,-1.0f, 1.0f);	// Bottom Left Of The Quad (Right)
                glVertex3f( 1.0f,-1.0f,-1.0f);	// Bottom Right Of The Quad (Right)
            glEnd();			// End Drawing The Cube
            glPopMatrix();

            /* Swap front and back buffers */
            glfwSwapBuffers(_window);
        }

        /* Poll for and process events */
        glfwPollEvents();
    }
}
