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
#include "simulation.h"

Simulation::Simulation():
    _world(NULL),
    _simulation_over(false),
    _world_filled(false),
    _initiated(false),
    _end_counter(0),
    _updates_since_last_step(0),
    _started(false),
    _elapsed_simulation(0),
    _paused(false)
{
}

Simulation::~Simulation(){
    if (_initiated){
        delete _world;
        delete _sequentialImpulseConstraintSolver;
        delete _broadphase;
        delete _dispatcher;
        delete _collisionConfiguration;
        for (int i = 0; i < _scenery.size(); ++i) {
            delete _scenery[i];
        }
    }
}

void Simulation::init(const SimulationParameters& params) {
    _params = params;
    allocateWorld();
    _initiated = true;
    _human.set_mass(params.get_body_mass());
    _human.loadObjects(params.get_input_location());
    _display = _human._limbs;
    Part *ground = allocateGround();
    _scenery.append(ground);
    resetStep();
    btRigidBody * body = ground->get_body();
    _world->addRigidBody(body);
}

Part * Simulation::allocateGround() const {
    Part * ground = new Part();
    ground->get_shape_struct().set_shape(btVector3(5,0.02,5));
    ground->set_mass(0); // no gravity
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0,-3,0));
    ground->set_original_transform(transform);
    return ground;
}

void Simulation::allocateWorld(){
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _broadphase = new btDbvtBroadphase();
    _sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

    _world = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_sequentialImpulseConstraintSolver,_collisionConfiguration);
    _world->setGravity(_params.get_gravity());
}

void Simulation::start(){
        _clock.reset();
        _thread = new QThread();
        moveToThread(_thread);
        if (!connect(_thread, SIGNAL(started()), this, SLOT(loop()))) qWarning()<<"Thread cannot be connected";
        qDebug()<<"Starting simulation";
        qDebug()<<"Parameters : \n\tDuration      : "<<_params.get_duration()<<"ms simulation"<<
                               "\n\tSteps duration: "<<_params.get_steps_duration()<<"ms"<<
                               "\n\tSpeed ratio   :  1/"<<_params.get_coefficient()<<"x"<<
                               "\n\tUPS           : "<<_params.get_ups();
        _thread->start();
        _started = true;
        qDebug()<<toString(_display[0]->get_body()->getCenterOfMassPosition());
}

void Simulation::loop(){
    double time_since_last_update = 0;

    btScalar coeff =          _params.get_coefficient();
    btScalar ups =            _params.get_ups();
    btScalar duration =       _params.get_duration();
    btScalar steps_duration = _params.get_steps_duration();

    btScalar clock_time = _clock.getTimeMilliseconds() / coeff ;
    _last_update_time = clock_time;
    while (_world && !_simulation_over){
        if(!_paused){
            clock_time = _clock.getTimeMilliseconds() / coeff ;
            time_since_last_update = clock_time - _last_update_time;
            if (time_since_last_update/1000 >= 1./(ups * coeff)){
                time_since_last_update = 0;
                _lock.lockForWrite(); {
                    update();
                    if (_step_counter > steps_duration)
                        stepOver();
                    if (_end_counter > duration)
                        simulationOver();
                    _updates_since_last_step++;
                } _lock.unlock();
                _last_update_time =clock_time;
            }
        }
    }
}

void Simulation::update(){

    btScalar coeff = _params.get_coefficient();
    btScalar ups = _params.get_ups();
    btScalar progression_s = 1.d/(ups * coeff);
    btScalar progression_ms = 1000.0d*(1./(ups * coeff));
    btScalar clock_time = _clock.getTimeMicroseconds()/coeff;

    _diff =  clock_time-_elapsed_realtime;
    _elapsed_realtime=clock_time;
    _elapsed_simulation=_elapsed_simulation+progression_ms;
    _world->stepSimulation(progression_s ,1,btScalar(1.0/(ups)));
    _step_counter+=progression_ms; // conversion from seconds to ms
    _end_counter+=progression_ms;
    _ups_counter+=progression_ms;
    _human.updateInformationJointTree(_elapsed_simulation,progression_ms,_params.get_gravity().y());
}

void Simulation::resetStep(float time){
    cleanWorld();
    _human.setSimulationPositionJointTree(time);
    fillWorld();
    _updates_since_last_step = 0;
    _step_counter = 0;
}

void Simulation::stepOver(){
    _human.recordStatus();
    resetStep(_elapsed_simulation);
}

void Simulation::cleanWorld(){
    btRigidBody * body;
    QList<Joint*> * _joints = &_human._joints;
    QList<Constraint> * _constraints_list= &_human._constraints;
    Constraint * temp_constraint;
    if (_world_filled){
        for (int i = 0; i < _constraints_list->size(); ++i) {
            temp_constraint = &((*_constraints_list)[i]);
            if (temp_constraint->has_parts())
                _world->removeConstraint(temp_constraint->get_constraint());
        }
        for (int i = 0; i < _display.size(); ++i) {
            body = _display[i]->get_body();
            _world->removeRigidBody(body);
        }
        _world_filled = false; //indicates that the world is now empty
    } else qWarning()<<"Attempting to clean an empty world" ;
}

void Simulation::fillWorld(){
    btRigidBody * body = NULL;
    QList<Constraint> * _constraints_list= &_human._constraints;
    Constraint * temp_constraint;
    if (!_world_filled){
        for (int i = 0; i < _display.size(); ++i) {
            body = _display[i]->get_body();
            _world->addRigidBody(body);
        }
        for (int i = 0; i < _constraints_list->size(); ++i) {
            temp_constraint = &((*_constraints_list)[i]);
//            if (!temp_constraint->is_constraint_allocated())
                temp_constraint->buildConstraint();
            if (temp_constraint->has_parts()){
                _world->addConstraint(temp_constraint->get_constraint(),true);
            }
        }
        _world_filled = true;
    } else qWarning()<<"Attempting to fill a full world" ;
}


void Simulation::simulationOver()
{
    //Setting path to save files
    QString path = "output/";
    if (QDir::setCurrent(path))
        qDebug()<<"path set";
    else
        qDebug()<<"path not set "<<path;

     _human.saveDataList();
     _human.saveFullDataList(_params);
     _human.saveCompleteDataList();
     _simulation_over = true;
     qDebug()<<"\n\nSimulation over";
     if (_params.get_automatic_close())
        QApplication::exit();
}
