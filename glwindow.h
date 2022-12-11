#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWindow();
    ~GLWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
};
#endif // GLWINDOW_H
