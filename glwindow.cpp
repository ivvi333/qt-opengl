#include "glwindow.h"

void getQuadVertices(QVector3D *vertices, float width, float length, float height, float x, float y, float z)
{
    float hw = width / 2, hl = length / 2, hh = height / 2;

    // Передняя грань
    vertices[0] = { x - hw, y + hl, z + hh };
    vertices[1] = { x - hw, y - hl, z + hh };
    vertices[2] = { x + hw, y - hl, z + hh };
    vertices[3] = { x + hw, y + hl, z + hh };

    // Задняя грань
    vertices[4] = { x + hw, y + hl, z - hh };
    vertices[5] = { x + hw, y - hl, z - hh };
    vertices[6] = { x - hw, y - hl, z - hh };
    vertices[7] = { x - hw, y + hl, z - hh };

    // Верхняя грань
    vertices[8] = { x - hw, y + hl, z - hh };
    vertices[9] = { x - hw, y + hl, z + hh };
    vertices[10] = { x + hw, y + hl, z + hh };
    vertices[11] = { x + hw, y + hl, z - hh };

    // Нижняя грань
    vertices[12] = { x + hw, y - hl, z - hh };
    vertices[13] = { x + hw, y - hl, z + hh };
    vertices[14] = { x - hw, y - hl, z + hh };
    vertices[15] = { x - hw, y - hl, z - hh };

    // Правая грань
    vertices[16] = { x + hw, y - hl, z + hh };
    vertices[17] = { x + hw, y - hl, z - hh };
    vertices[18] = { x + hw, y + hl, z - hh };
    vertices[19] = { x + hw, y + hl, z + hh };

    // Левая грань
    vertices[20] = { x - hw, y + hl, z + hh };
    vertices[21] = { x - hw, y + hl, z - hh };
    vertices[22] = { x - hw, y - hl, z - hh };
    vertices[23] = { x - hw, y - hl, z + hh };
}

GLWindow::GLWindow()
{
}

GLWindow::~GLWindow()
{
}

void GLWindow::resetProjection()
{
    projectMatrix.setToIdentity();
    projectMatrix.perspective(30.0, (float)width()/height(), 0.1, 100);
}

void GLWindow::resetModelView()
{
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0, 0, -zoffset);
    modelViewMatrix *= rotateMatrix.transposed();
    modelViewMatrix.scale(0.1, 0.1, 0.1);
}

void GLWindow::changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy)
{
    rotate_matrix.rotate(-dx, 0.0, 1.0, 0.0);
    rotate_matrix.rotate(-dy, 1.0, 0.0, 0.0);
}

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
        changeRotateMatrix(rotateMatrix, dp.x()*0.5, dp.y()*0.5);
        resetModelView();
        lastMousePosition = event->pos();
        update();
    }
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && dragging) {
        QPoint dp = event->pos() - lastMousePosition;
        changeRotateMatrix(rotateMatrix, dp.x()*0.5, dp.y()*0.5);
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
    glClearColor(0.9, 0.9, 0.9, 1.0);

    resetModelView();

    initShader();

    platform = std::make_unique<QVector3D[]>(24);
    getQuadVertices(platform.get(), 100.0, 2.0, 100.0, 0.0, -8.0, 0.0);
    trees.push_back(std::make_unique<QVector3D[]>(24));
    getQuadVertices(trees[trees.size() - 1].get(), 10.0, 40.0, 10.0, -30.0, 13.0, -30.0);
    trees.push_back(std::make_unique<QVector3D[]>(24));
    getQuadVertices(trees[trees.size() - 1].get(), 10.0, 40.0, 10.0, 30.0, 13.0, -30.0);
    trees.push_back(std::make_unique<QVector3D[]>(24));
    getQuadVertices(trees[trees.size() - 1].get(), 10.0, 40.0, 10.0, 0.0, 13.0, 0.0);
}

void GLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    resetProjection();
}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quadShaderProgram.bind();

    quadShaderProgram.setUniformValue("modelViewProjectionMatrix", projectMatrix * modelViewMatrix);

    drawPlatform();
    drawTrees();
}

void GLWindow::drawPlatform()
{
    quadShaderProgram.setUniformValue("objectColor", QVector3D(50.0/255.0, 75.0/255.0, 40.0/255.0));
    quadShaderProgram.setAttributeArray("vertex", platform.get());
    glDrawArrays(GL_QUADS, 0, 6*4);
}

void GLWindow::drawTrees()
{
    for (const auto &tree : trees) {
        quadShaderProgram.setUniformValue("objectColor", QVector3D(74.0/255.0, 53.0/255.0, 42.0/255.0));
        quadShaderProgram.setAttributeArray("vertex", tree.get());
        glDrawArrays(GL_QUADS, 0, 6*4);
    }
}
