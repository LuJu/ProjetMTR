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
class TestScene : public Viewer
{
    Q_OBJECT
public slots :
//void simulationOver();

public:
    ~TestScene();
//    TestScene(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 )
//        : Viewer ( parent, shareWidget,f ){}
    virtual void draw();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void closeEvent(QCloseEvent * event);
    virtual void display3DObjects();

//    virtual void closeEvent(QCloseEvent * event);
    void saveCurrentStatus();
    void displayAnimation();
    void displaySimulation();
    void displayStats();
    void installDebugger();

    int _type;

//    Simulation& get_simulation() {return _simulation;}
    Simulation  * _simulation;

private:
    Mesh _cube_mesh;
    Mesh _cylinder_mesh;
    Mesh _capsule_mesh;

    btDiscreteDynamicsWorld * _world;


    InteractiveObject _ground;
    InteractiveObject * _small;

};

#endif // TESTSCENE_H
