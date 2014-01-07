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
    void installDebugger();
private:
    Mesh _cube_mesh;
    Mesh _cylinder_mesh;

    Simulation _simulation;
    btDiscreteDynamicsWorld * _world;


    InteractiveObject _ground;
    InteractiveObject * _small;


    QWidget debugging;
    static DebuggingInterface debugging_ui;

    static void customMessageHandler(QtMsgType type, const char *msg){
        debugging_ui.console->append(msg);
    }
};

#endif // TESTSCENE_H
