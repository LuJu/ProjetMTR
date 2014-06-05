/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SIMULATIONPARAMETERS_H
#define SIMULATIONPARAMETERS_H

#include <QString>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

//! Class representing the parameters of a simulation : duration, steps duration, gravity etc
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
    btScalar get_body_mass() const {return _body_mass;}
    void set_body_mass(int body_mass){_body_mass = body_mass;}
    const QString& get_input_location() const {return _input_location;}
    void set_input_location(const QString& input_location){_input_location = input_location;}
    bool get_automatic_close() const {return _automatic_close;}
    void set_automatic_close(int automatic_close){_automatic_close = automatic_close;}

private:
    btScalar _steps_duration;
    btScalar _body_mass;
    btScalar _duration;
    btScalar _coefficient;
    btScalar _ups;
    btVector3 _gravity;
    QString _input_location;

    bool _automatic_close;

};

#endif // SIMULATIONPARAMETERS_H
