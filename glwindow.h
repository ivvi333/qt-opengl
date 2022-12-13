#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

    float zoffset = 3;

    QPoint lastMousePosition;
    bool dragging;

    QMatrix4x4 rotateMatrix;
    QMatrix4x4 modelViewMatrix;
    QMatrix4x4 projectMatrix;

    QOpenGLShaderProgram quadShaderProgram;

    std::unique_ptr<QVector3D[]> platform;
    std::vector<std::unique_ptr<QVector3D[]>> trees;

    void resetProjection();
    void resetModelView();
    static void changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy);

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    void initShader();

    void drawPlatform();
    void drawTrees();

public:
    GLWindow();
    ~GLWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
#endif // GLWINDOW_H
