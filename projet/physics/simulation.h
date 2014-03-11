#ifndef SIMULATION_H
#define SIMULATION_H()


#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include <QTimer>
#include <QTextStream>
#include <QObject>
#include <QWidget>
#include <QVector>
#include <QReadWriteLock>
#include <QThread>
#include <QApplication>

#include "utils/csvparser.h"

#include "bodyinfo.h"
#include "humanbody.h"
#include "simulationparameters.h"
#include "interactiveobject.h"
#include "joint.h"

class Simulation : public QObject
{
    Q_OBJECT
public:

    Simulation();
    ~Simulation();
    void init(const SimulationParameters &params);
    void start();
    void stop(){_simulation_over = true;}

    const QList<InteractiveObject * >& get_display_list() {return  _display;}
    bool is_over() const {return _simulation_over;}
    float get_elapsed_microseconds() const {return _elapsed;}
    float get_elapsed_milliseconds() const {return _elapsed/1000;}
    HumanBody * get_human() {return &_human;}
    QReadWriteLock* get_lock() {return &_lock;}

    bool is_started() const {return _started;}

public slots:
    void loop();
private:
    void update();
    void allocateWorld();
    void simulationOver();
    void stepOver();

    void resetStep(float time = 0);
    //! function called each step, empties the world
    void cleanWorld();

    //! fills the world with all the objects of the simulation
    void fillWorld();

    SimulationParameters _params;
    QReadWriteLock _lock;

    btDiscreteDynamicsWorld * _world;
    btBroadphaseInterface *_broadphase;
    btCollisionDispatcher *_dispatcher;
    btDefaultCollisionConfiguration *_collisionConfiguration;
    btSequentialImpulseConstraintSolver *_sequentialImpulseConstraintSolver;
    InteractiveObject * _ground;

    QThread * _thread;
    HumanBody _human;
    QList<InteractiveObject * > _display;
    QList<Joint> _joints;



    btScalar _elapsed;
    btScalar _elapsed2;
    btScalar _diff;
//    double _time_beginning;


    btScalar _step_counter;
    btScalar _end_counter;
    btScalar _ups_counter;
    btScalar _last_update_time;

    int _updates_since_last_step;


    bool _simulation_over;
    bool _world_filled;
    btClock _clock;

    //! indicates that the init() function has been called
    bool _initiated;

    //! indicates that the start() function has been called
    bool _started;
};

#endif // SIMULATION_H
