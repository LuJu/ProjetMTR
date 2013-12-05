#ifndef SIMULATION_H
#define SIMULATION_H()


#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "simulationparameters.h"
#include "interactiveobject.h"
#include "core/core.h"
#include "utils/csvparser.h"
#include  <QTimer>
#include <QTextStream>
#include <QObject>
#include <QVector>
#include "bodyinfo.h"
#include "humanbody.h"

class Simulation : public QObject
{
    Q_OBJECT
public slots:
    void simulationOver();
    void autoloopSteps();
    void stepOver();
public:

    Simulation();
    ~Simulation();
    void standard();
    void initiate(const SimulationParameters& params);
    void update();
    void loop();
    void autoloopStart();
    void cleanWorld();
    void startSimulation();
    void resetStep(float time = 0);
    void deleteTimer(QTimer * timer);
    void fillWorld();

    QList<InteractiveObject * >& get_display_list(){return  _display;}
    btDiscreteDynamicsWorld * get_world() {return _world;}
    void set_world(btDiscreteDynamicsWorld * world){_world = world;}
    bool is_going() const {return _going;}
    bool is_over() const {return _over;}
    void set_autoloop(bool autoloop){_autoloop=autoloop;}

    float get_elapsed() const {return _elapsed;}
    float get_time_simulation() const {return _elapsed;}
    float _diff;
    void calculateWork();

private:

    bool is_autoloop() const {return _autoloop;}
    void saveDataList();
    void deleteData();

    QList<InteractiveObject::energy_info> _data_list;
    SimulationParameters _params;

    btDiscreteDynamicsWorld * _world;
    btBroadphaseInterface *_broadphase;
    btCollisionDispatcher *_dispatcher;
    btDefaultCollisionConfiguration *_collisionConfiguration;
    btSequentialImpulseConstraintSolver *_sequentialImpulseConstraintSolver;

    QList<InteractiveObject * > _display;
    InteractiveObject * _ground;
    QTimer * _timer_simulations;
    QTimer * _timer_autoloop;
    QTimer * _timer_steps;
    float _elapsed;
    float _time_beginning;



    bool _over;
    bool _going;
    bool _autoloop;
    bool _initiated;
    bool _world_filled;
    btClock _clock;

    HumanBody _human;
};

#endif // SIMULATION_H
