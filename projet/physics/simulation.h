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


//! Main class of the program: runs the simulation and the animation
class Simulation : public QObject
{
    Q_OBJECT
public:

    Simulation();
    ~Simulation();

    //! Initiates the simulation: loads object data, etc
    void init(const SimulationParameters &params);

    const QList<InteractiveObject * >& get_display_list() {return  _display;}
    const QList<InteractiveObject * >& get_scenery() {return  _scenery;}
    float get_elapsed_realtime() const {return _elapsed_realtime;} //! Returns the realtime elapsed since the start of the simulation
    float get_elapsed_milliseconds() const {return _elapsed_simulation;} //! Returns the simulation time elapsed since the start of the simulation


    HumanBody * get_human() {return &_human;}
    QReadWriteLock* get_lock() {return &_lock;}

    void start();
    void stop(){_simulation_over = true;}
    bool is_started() const   {return _started;}
    bool is_initiated() const {return _initiated;}
    bool is_over() const      {return _simulation_over;}

private slots:
    //! Thread that will run continually until the end of the simulation
    void loop();
private:
    void update();
    void allocateWorld();
    //! Function called when the simulation is over
    void simulationOver();

    InteractiveObject * allocateGround() const;

    //! Function called when a step is over
    void stepOver();
    //! Function called when the simulation has to be replaced at the corresponding time of the animation
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

    QThread * _thread;
    HumanBody _human;
    //animated objects
    QList<InteractiveObject * > _display;
    //unanimated objects
    QList<InteractiveObject * > _scenery;

    btScalar _elapsed_realtime;
    btScalar _elapsed_simulation;
    btScalar _diff;


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
