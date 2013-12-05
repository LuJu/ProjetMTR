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
    int get_duration() const {return _duration;}
    void set_duration(int duration){_duration = duration;}
    int get_total_duration() const {return _total_duration;}
    void set_total_duration(int total_duration){_total_duration = total_duration;}
    int get_coefficient() const {return _coefficient;}
    void set_coefficient(int coefficient){_coefficient = coefficient;}
    int get_ups() const {return _ups;}
    void set_ups(int ups){_ups = ups;}
    float _steps_duration;

private:
    int _duration;
    int _total_duration;
    int _coefficient;
    int _ups;
    btVector3 _gravity;

};

#endif // SIMULATIONPARAMETERS_H
