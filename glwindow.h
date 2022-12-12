#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vbo;

public:
    GLWindow();
    ~GLWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
#endif // GLWINDOW_H
