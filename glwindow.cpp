#include "glwindow.h"

GLWindow::GLWindow()
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::resetProjection()
{
    projectMatrix.setToIdentity();
    projectMatrix.perspective(30.0, (float)width()/height(), 0.1, 20);
}

void GLWindow::resetModelView()
{
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0, 0, -zoffset);
    modelViewMatrix *= rotateMatrix.transposed();
    modelViewMatrix.scale(0.3, 0.3, 0.3);
}

void GLWindow::changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy)
{
    rotate_matrix.rotate(-dx, 0.0, 1.0, 0.0);
    rotate_matrix.rotate(-dy, 1.0, 0.0, 0.0);
}

//void GLWindow::mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint dp = event->pos() - lastMousePosition;
//    changeRotateMatrix(rotateMatrix, dp.x(), dp.y());
//    lastMousePosition = event->pos();
//    resetModelView();
//    update();
//}

void GLWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8.0;
    if (!numDegrees.isNull()) {
        zoffset -= numDegrees.y() * 0.03;
        resetModelView();
        update();
    }
}

void GLWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->pos();
        dragging = true;
    }
}

void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && dragging) {
        QPoint dp = event->pos() - lastMousePosition;
        changeRotateMatrix(rotateMatrix, dp.x(), dp.y());
        resetModelView();
        lastMousePosition = event->pos();
        update();
    }
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && dragging) {
        QPoint dp = event->pos() - lastMousePosition;
        changeRotateMatrix(rotateMatrix, dp.x(), dp.y());
        resetModelView();
        dragging = false;
        update();
    }
}

void GLWindow::initShader()
{
    quadShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/quadVertShader.vert");
    quadShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/quadFragShader.frag");
    if (quadShaderProgram.link() == false)
        qDebug() << quadShaderProgram.log();
    quadShaderProgram.enableAttributeArray("vertex");
}


void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.25f, 0.25f, 0.25f, 1.f);

    resetModelView();

    initShader();
}

void GLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    resetProjection();
}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const float vertices[6*4][3] =
                    {{ -1.0f,  1.0f,  1.0f}, { -1.0f,  -1.0f,  1.0f}, {  1.0f, -1.0f,  1.0f}, {  1.0f,  1.0f,  1.0f},  // Передняя грань (z =  1)
                     {  1.0f,  1.0f, -1.0f}, {  1.0f,  -1.0f, -1.0f}, { -1.0f, -1.0f, -1.0f}, { -1.0f,  1.0f, -1.0f},  // Задняя грань   (z = -1)
                     {  1.0f,  1.0f, -1.0f}, { -1.0f,   1.0f, -1.0f}, { -1.0f,  1.0f,  1.0f}, {  1.0f,  1.0f,  1.0f},  // Верхняя грань  (y =  1)
                     {  1.0f, -1.0f,  1.0f}, { -1.0f,  -1.0f,  1.0f}, { -1.0f, -1.0f, -1.0f}, {  1.0f, -1.0f, -1.0f},  // Нижняя грань   (y = -1)
                     {  1.0f, -1.0f,  1.0f}, {  1.0f,  -1.0f, -1.0f}, {  1.0f,  1.0f, -1.0f}, {  1.0f,  1.0f,  1.0f},  // Правая грань   (x =  1)
                     { -1.0f,  1.0f,  1.0f}, { -1.0f,   1.0f, -1.0f}, { -1.0f, -1.0f, -1.0f}, { -1.0f, -1.0f,  1.0f}}; // Левая грань    (x = -1)

    quadShaderProgram.bind();

    quadShaderProgram.setUniformValue("modelViewProjectionMatrix", projectMatrix * modelViewMatrix);
    quadShaderProgram.setUniformValue("objectColor", QVector3D(1.0, 0.0, 0.0));

    quadShaderProgram.setAttributeArray("vertex", (float*)vertices, 3);

    glDrawArrays(GL_QUADS, 0, 6*4);
}
