#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <QObject>
#include <QVector>
#include <QRect>
#include "viewer.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "physics/interactiveobject.h"
#include "physics/simulation.h"
#include "utils/maths.h"
#include "utils/csvparser.h"
class TestScene : public Viewer
{
    Q_OBJECT
public slots :
//void simulationOver();

public:
    ~TestScene();
    virtual void draw();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    virtual void keyReleaseEvent(QKeyEvent *keyEvent);
    virtual void display3DObjects();
//    virtual void closeEvent(QCloseEvent * event);
    void saveCurrentStatus();
    void displayAnimation();
    void displaySimulation();
    void displayStats();
private:
    Mesh _cube_mesh;
    Mesh _cylinder_mesh;

    Simulation _simulation;
    btDiscreteDynamicsWorld * _world;


    InteractiveObject _ground;
    InteractiveObject * _small;

};

#endif // TESTSCENE_H
