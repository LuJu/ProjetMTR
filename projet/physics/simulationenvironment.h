#ifndef SIMULATIONENVIRONMENT_H
#define SIMULATIONENVIRONMENT_H


#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "QDebug"

//! Contains the objects to run a bullet simulation environment
class SimulationEnvironment
{
public:
    SimulationEnvironment(btVector3 gravity);
    ~SimulationEnvironment();
    //! Return a pointer to the world which is allocated in the constructor
    btDiscreteDynamicsWorld * get_world(){return _world;}
private:
    SimulationEnvironment(SimulationEnvironment& other); // disable copy
    SimulationEnvironment& operator=( const SimulationEnvironment& other ) ;

    btDiscreteDynamicsWorld * _world;
    btBroadphaseInterface *_broadphase;
    btCollisionDispatcher *_dispatcher;
    btDefaultCollisionConfiguration *_collision_configuration;
    btSequentialImpulseConstraintSolver *_sequential_impulse_constraint_solver;
};

#endif // SIMULATIONENVIRONMENT_H
