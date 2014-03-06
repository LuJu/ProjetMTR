#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <QObject>
#include <QVector>
#include <QRect>
#include "core/viewer.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "physics/interactiveobject.h"
#include "physics/simulation.h"
#include "physics/debugginginterface.h"
#include "utils/maths.h"
#include "utils/csvparser.h"
#include "3d/meshutils.h"
class TestScene : public Viewer
{
    Q_OBJECT
public slots :
//void simulationOver();

public:
    ~TestScene();
    TestScene(QGLContext * context);
    TestScene();
    TestScene(QWidget * parent);
//    TestScene(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 )
//        : Viewer ( parent, shareWidget,f ){}
    virtual void draw();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void closeEvent(QCloseEvent * event);
    virtual void display3DObjects();

    void displayAnimation();
    void displaySimulation();
    Simulation  * _simulation;

private:
    Mesh _cube_mesh;
    Mesh _cylinder_mesh;

    QList<InteractiveObject * > _display;

};

#endif // TESTSCENE_H
