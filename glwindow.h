#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QMatrix4x4>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#define MAX_LAMP_COUNT 4

class Tree {
    std::unique_ptr<QVector3D[]> treeTrunc, treeTruncNorm;
    std::unique_ptr<QOpenGLTexture> treeTruncTexture;
    std::unique_ptr<QVector2D[]> treeTruncTextureCoord;

    std::vector<std::unique_ptr<QVector3D[]>> treeLeaves, treeLeavesNorm;
    std::unique_ptr<QOpenGLTexture> treeLeavesTexture;
    std::unique_ptr<QVector2D[]> treeLeavesTextureCoord;

    int type;

public:
    Tree(int type, float width, float length, float height, float x, float y, float z);
    QVector3D* getTreeTrunc() const { return treeTrunc.get(); }
    QVector3D* getTreeTruncNorm() const { return treeTruncNorm.get(); }
    QOpenGLTexture* getTreeTruncTexture() const { return treeTruncTexture.get(); }
    QVector2D* getTreeTruncTextureCoord() const { return treeTruncTextureCoord.get(); }

    std::vector<std::unique_ptr<QVector3D[]>> const& getTreeLeaves() const { return treeLeaves; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getTreeLeavesNorm() const { return treeLeavesNorm; }
    QOpenGLTexture* getTreeLeavesTexture() const { return treeLeavesTexture.get(); }
    QVector2D* getTreeLeavesTextureCoord() const { return treeLeavesTextureCoord.get(); }
};

class House {
    std::unique_ptr<QVector3D[]> houseBase, houseBaseNorm;
    std::unique_ptr<QOpenGLTexture> houseBaseTexture;
    std::unique_ptr<QVector2D[]> houseBaseTextureCoord;

    std::vector<std::unique_ptr<QVector3D[]>> houseRoof, houseRoofNorm;
    std::unique_ptr<QOpenGLTexture> houseRoofTexture;
    std::unique_ptr<QVector2D[]> houseRoofTextureCoord;

    int type;

public:
    House(int type, float width, float length, float height, float x, float y, float z);
    QVector3D* getHouseBase() const { return houseBase.get(); }
    QVector3D* getHouseBaseNorm() const { return houseBaseNorm.get(); }
    QOpenGLTexture* getHouseBaseTexture() const { return houseBaseTexture.get(); }
    QVector2D* getHouseBaseTextureCoord() const { return houseBaseTextureCoord.get(); }

    std::vector<std::unique_ptr<QVector3D[]>> const& getHouseRoof() const { return houseRoof; }
    std::vector<std::unique_ptr<QVector3D[]>> const& getHouseRoofNorm() const { return houseRoofNorm; }
    QOpenGLTexture* getHouseRoofTexture() const { return houseRoofTexture.get(); }
    QVector2D* getHouseRoofTextureCoord() const { return houseRoofTextureCoord.get(); }
};

class Lamp {
    std::unique_ptr<QVector3D[]> pole, poleNorm;
    std::unique_ptr<QOpenGLTexture> poleTexture;
    std::unique_ptr<QVector2D[]> poleTextureCoord;

    std::unique_ptr<QVector3D[]> head, headNorm;
    std::unique_ptr<QOpenGLTexture> headTexture;
    std::unique_ptr<QVector2D[]> headTextureCoord;

    QVector3D getCenter(QVector3D *vertices, int size) const;

public:
    Lamp(float width, float length, float height, float x, float y, float z);
    QVector3D* getPole() const { return pole.get(); }
    QVector3D* getPoleNorm() const { return poleNorm.get(); }
    QVector3D getPoleCenter() const { return getCenter(pole.get(), 24); }
    QOpenGLTexture* getPoleTexture() const { return poleTexture.get(); }
    QVector2D* getPoleTextureCoord() const { return poleTextureCoord.get(); }

    QVector3D* getHead() const { return head.get(); }
    QVector3D* getHeadNorm() const { return headNorm.get(); }
    QVector3D getHeadCenter() const { return getCenter(head.get(), 24); }
    QOpenGLTexture* getHeadTexture() const { return headTexture.get(); }
    QVector2D* getHeadTextureCoord() const { return headTextureCoord.get(); }
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

    std::unique_ptr<QOpenGLTexture> platformTexture;
    std::unique_ptr<QVector2D[]> platformTextureCoord;
    const float platformW = 1000.0, platformL = platformW, platformH = 10.0;
    std::unique_ptr<QVector3D[]> platform, platformNorm;
    const float cellW = 50.0, cellL = cellW;
    std::vector<std::pair<int, int>> freeCells;

    float zoffset = platformW / 8.0;

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
