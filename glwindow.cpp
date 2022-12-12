#include "glwindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

GLWindow::GLWindow()
{
}

GLWindow::~GLWindow()
{
    delete vao;
    delete vbo;
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    GLfloat vpoint[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    vao = new QOpenGLVertexArrayObject;
    vao->create();
    vao->bind();
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(&vpoint, sizeof(vpoint));
}

void GLWindow::resizeGL(int w, int h)
{

}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QOpenGLShaderProgram shaderProg;
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, QString(":/testVShader.vert"));
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, QString(":/testFShader.frag"));
    shaderProg.link();
    shaderProg.bind();
    shaderProg.setAttributeBuffer("vpoint", GL_FLOAT, 0, 3);
    shaderProg.enableAttributeArray("vpoint");

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
