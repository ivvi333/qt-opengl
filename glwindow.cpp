#include "glwindow.h"

GLWindow::GLWindow()
    : QOpenGLWindow()
{
    setSurfaceType(QWindow::OpenGLSurface);
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{

}

void GLWindow::resizeGL(int w, int h)
{

}

void GLWindow::paintGL()
{

}

void GLWindow::paintEvent(QPaintEvent *event)
{

}

void GLWindow::resizeEvent(QResizeEvent *event)
{

}
