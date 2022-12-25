#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#define MAX_LAMP_COUNT 4

class Tree {
    std::unique_ptr<QVector3D[]> treeTrunc, treeTruncNorm;
    QVector3D treeTruncColor;
    std::vector<std::unique_ptr<QVector3D[]>> treeLeaves, treeLeavesNorm;
    QVector3D treeLeavesColor;
    int type;

public:
    Tree(int type, float width, float length, float height, float x, float y, float z);
    QVector3D* getTreeTrunc() const { return treeTrunc.get(); }
    QVector3D* getTreeTruncNorm() const { return treeTruncNorm.get(); }
    QVector3D getTreeTruncColor() const { return treeTruncColor; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getTreeLeaves() const { return treeLeaves; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getTreeLeavesNorm() const { return treeLeavesNorm; }
    QVector3D getTreeLeavesColor() const { return treeLeavesColor; }
};

class House {
    std::unique_ptr<QVector3D[]> houseBase, houseBaseNorm;
    QVector3D houseBaseColor;
    std::vector<std::unique_ptr<QVector3D[]>> houseRoof, houseRoofNorm;
    QVector3D houseRoofColor;
    int type;

public:
    House(int type, float width, float length, float height, float x, float y, float z);
    QVector3D* getHouseBase() const { return houseBase.get(); }
    QVector3D* getHouseBaseNorm() const { return houseBaseNorm.get(); }
    QVector3D getHouseBaseColor() const { return houseBaseColor; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getHouseRoof() const { return houseRoof; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getHouseRoofNorm() const { return houseRoofNorm; }
    QVector3D getHouseRoofColor() const { return houseRoofColor; }
};

class Lamp {
    std::unique_ptr<QVector3D[]> pole, poleNorm;
    QVector3D poleColor;
    std::unique_ptr<QVector3D[]> head, headNorm;
    QVector3D headColor;

    QVector3D getCenter(QVector3D *vertices, int size) const;

public:
    Lamp(float width, float length, float height, float x, float y, float z);
    QVector3D* getPole() const { return pole.get(); }
    QVector3D* getPoleNorm() const { return poleNorm.get(); }
    QVector3D getPoleCenter() const { return getCenter(pole.get(), 24); }
    QVector3D getPoleColor() const { return poleColor; }
    QVector3D* getHead() const { return head.get(); }
    QVector3D* getHeadNorm() const { return headNorm.get(); }
    QVector3D getHeadCenter() const { return getCenter(head.get(), 24); }
    QVector3D getHeadColor() const { return headColor; }
};

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
    QPointF lastMousePosition;
    bool dragging;

    QMatrix4x4 rotateMatrix;
    float angle = 30.0;
    QMatrix4x4 modelViewMatrix;
    QMatrix4x4 projectMatrix;

    QOpenGLShaderProgram quadShaderProgram;

    const QVector3D platformColor = QVector3D(200.0/255.0, 210.0/255.0, 195.0/255.0);
    const float platformW = 10000.0, platformL = platformW, platformH = 10.0;
    std::unique_ptr<QVector3D[]> platform, platformNorm;
    const float cellW = 50.0, cellL = cellW;
    std::vector<std::pair<int, int>> freeCells;

    float zoffset = platformW / 32.0;

    std::vector<House> houses;
    std::vector<Tree> trees;
    std::vector<Lamp> lamps;
    int lampCount;

    void addLamp();
    void addTree();
    void addHouse();

    void freeAllCells();

    void resetProjection();
    void resetModelView();
    static void changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy);

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void initShader();

    void drawPlatform();
    void drawTrees();
    void drawHouses();
    void drawLamps();

public:
    GLWindow();
    ~GLWindow();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
#endif // GLWINDOW_H
