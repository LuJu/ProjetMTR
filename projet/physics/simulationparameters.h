#ifndef SIMULATIONPARAMETERS_H
#define SIMULATIONPARAMETERS_H

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/geometry.h"

class SimulationParameters
{
public:
    SimulationParameters(){}

    btVector3 get_gravity() const {return _gravity;}
    void set_gravity(btVector3 gravity){_gravity = gravity;}
    btScalar get_duration() const {return _duration;}
    void set_duration(int duration){_duration = duration;}
    btScalar get_coefficient() const {return _coefficient;}
    void set_coefficient(int coefficient){_coefficient = coefficient;}
    btScalar get_ups() const {return _ups;}
    void set_ups(int ups){_ups = ups;}
    btScalar get_steps_duration() const {return _steps_duration;}
    void set_steps_duration(int steps_duration){_steps_duration = steps_duration;}

private:
    btScalar _steps_duration;
    btScalar _duration;
    btScalar _coefficient;
    btScalar _ups;
    btVector3 _gravity;

};

#endif // SIMULATIONPARAMETERS_H
