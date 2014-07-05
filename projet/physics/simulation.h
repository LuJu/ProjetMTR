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
#include "simulationenvironment.h"
#include "part.h"
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

    const QList<Part * >& get_display_list() const {return  _human._limbs;}
    const QList<Part * >& get_scenery() const {return  _scenery;}
    float get_elapsed_realtime() const {return _elapsed_realtime;} //! Returns the realtime elapsed since the start of the simulation
    float get_elapsed_milliseconds() const {return _elapsed_simulation;} //! Returns the simulation time elapsed since the start of the simulation

    QReadWriteLock* get_lock() {return &_lock;}

    void start();
    void stop(){_simulation_over = true;}
    bool is_started() const   {return _started;}
    bool is_initiated() const {return _initiated;}
    bool is_over() const      {return _simulation_over;}
    bool is_paused() const { return _paused;}
    void set_paused(bool paused){_paused = paused;}
    bool _paused;

private slots:
    //! Thread that will run continually until the end of the simulation
    void loop();
private:
    void update();

    //! Function called when the simulation is over
    void simulationOver();

    Part * allocateGround() const;

    //! Function called when a step is over
    void stepOver();
    //! Function called when the simulation has to be replaced at the corresponding time of the animation
    void resetStep(float time = 0);
    //! function called each step, empties the world
    void cleanWorld();
    //! fills the world with all the objects of the simulation
    void fillWorld();

    SimulationParameters _params;
    SimulationEnvironment * _environment;

    QReadWriteLock _lock;

    QThread * _thread;

    HumanBody _human;

    //unanimated objects
    QList<Part * > _scenery;

    btScalar _elapsed_realtime;
    btScalar _elapsed_simulation;
    btScalar _diff;


    btScalar _step_counter;
    btScalar _delta_counter;
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
