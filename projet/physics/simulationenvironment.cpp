#include "simulationenvironment.h"

SimulationEnvironment::SimulationEnvironment(btVector3 gravity)
{
    _collision_configuration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collision_configuration);
    _broadphase = new btDbvtBroadphase();
    _sequential_impulse_constraint_solver = new btSequentialImpulseConstraintSolver;

    _world = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_sequential_impulse_constraint_solver,_collision_configuration);
    _world->setGravity(gravity);
}

SimulationEnvironment::~SimulationEnvironment(){
    delete _world;
    delete _sequential_impulse_constraint_solver;
    delete _broadphase;
    delete _dispatcher;
    delete _collision_configuration;
}
