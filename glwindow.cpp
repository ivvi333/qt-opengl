#include "glwindow.h"

#include <random>

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

Tree::Tree(int type, float width, float length, float height, float x, float y, float z)
    : type(type)
{
    treeTruncColor = QVector3D(74.0/255.0, 53.0/255.0, 42.0/255.0);

    treeTrunc = std::make_unique<QVector3D[]>(24);
    getQuadVertices(treeTrunc.get(), width / 3, length / 3, height - height / 12, x, y, z - height / 24);

    if (width > length)
        length = width;
    else
        width = length;

    switch (type) {
    case 1:
        treeLeavesColor = QVector3D(220.0/255.0, 225.0/255.0, 120.0/255.0);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, height / 2, x, y, z + height / 4);
        break;

    case 2:
        treeLeavesColor = QVector3D(40.0/255.0, 80.0/255.0, 25.0/255.0);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, 3 * height / 4, x, y, z + height / 8);
        break;

    case 3:
        treeLeavesColor = QVector3D(20.0/255.0, 35.0/255.0, 20.0/255.0);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, height / 6, x, y, z + 5 * height / 12);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, height / 6, x, y, z + 2 * height / 12);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, height / 6, x, y, z - height / 12);
        treeLeaves.push_back(std::make_unique<QVector3D[]>(24));
        getQuadVertices(treeLeaves[treeLeaves.size() - 1].get(), width, length, height / 6, x, y, z - 4 * height / 12);
        break;
    }
}

void GLWindow::addTree() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> cellDis(0, freeCells.size() - 1);
    int cellIndex = cellDis(gen);

    std::uniform_int_distribution<int> typeDis(1, 3);
    int type = typeDis(gen);

    std::uniform_real_distribution<float> widthDis(10.0, 45.0);
    float width = widthDis(gen);

    std::uniform_real_distribution<float> lengthDis(10.0, 45.0);
    float length = lengthDis(gen);

    std::uniform_real_distribution<float> heightDis(20.0, 70.0);
    float height = heightDis(gen);

    trees.push_back(Tree(type, width, length, height,
                         cellW / 2 + cellW * freeCells[cellIndex].first - platformW / 2,
                         cellL / 2 + cellL * freeCells[cellIndex].second - platformL / 2,
                         platformH / 2 + height / 2));
    freeCells.erase(freeCells.begin() + cellIndex);
}

void GLWindow::freeAllCells() {
    freeCells.clear();
    for (int i = 0; i < platformW / cellW; i++)
        for (int j = 0; j < platformL / cellL; j++)
            freeCells.push_back(std::pair<int, int>(i, j));
}

void GLWindow::resetProjection()
{
    projectMatrix.setToIdentity();
    projectMatrix.perspective(30.0, (float)width()/height(), 0.1,
                              (platformL > platformW ? platformL * 2 : platformW * 2));
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

void GLWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->position();
        dragging = true;
    }
}

void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && dragging) {
        QPointF dp = event->position() - lastMousePosition;
        changeRotateMatrix(rotateMatrix, dp.rx()*0.5, dp.ry()*0.5);
        resetModelView();
        lastMousePosition = event->position();
        update();
    }
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && dragging) {
        QPointF dp = event->position() - lastMousePosition;
        changeRotateMatrix(rotateMatrix, dp.rx()*0.5, dp.ry()*0.5);
        resetModelView();
        dragging = false;
        update();
    }
}

void GLWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8.0;
    if (!numDegrees.isNull()) {
        zoffset -= numDegrees.y() * (platformL > platformW ? platformL * 0.0005 : platformW * 0.0005);
        resetModelView();
        update();
    }
}

void GLWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_R:
            std::random_device rd;
            std::mt19937 gen(rd());

            freeAllCells();
            trees.clear();
            std::uniform_int_distribution<int> dis(freeCells.size() / 4, freeCells.size() / 2);

            int treeCount = dis(gen);
            for (int i = 0; i < treeCount; i++)
                addTree();
            update();
            break;
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


GLWindow::GLWindow()
{
    platform = std::make_unique<QVector3D[]>(24);
    getQuadVertices(platform.get(), platformW, platformL, platformH, 0.0, 0.0, 0.0);

    freeAllCells();



}

GLWindow::~GLWindow()
{
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.9, 0.9, 0.9, 1.0);

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

    quadShaderProgram.bind();

    quadShaderProgram.setUniformValue("modelViewProjectionMatrix", projectMatrix * modelViewMatrix);

    drawPlatform();
    drawTrees();
}

void GLWindow::drawPlatform()
{
    quadShaderProgram.setUniformValue("objectColor", QVector3D(200.0/255.0, 210.0/255.0, 195.0/255.0));
    quadShaderProgram.setAttributeArray("vertex", platform.get());
    glDrawArrays(GL_QUADS, 0, 6*4);
}

void GLWindow::drawTrees()
{
    for (const auto &tree : trees) {
        quadShaderProgram.setUniformValue("objectColor", tree.getTreeTruncColor());
        quadShaderProgram.setAttributeArray("vertex", tree.getTreeTrunc());
        glDrawArrays(GL_QUADS, 0, 6*4);

        quadShaderProgram.setUniformValue("objectColor", tree.getTreeLeavesColor());
        for (auto &leaf : tree.getTreeLeaves()) {
            quadShaderProgram.setAttributeArray("vertex", leaf.get());
            glDrawArrays(GL_QUADS, 0, 6*4);
        }
    }
}
