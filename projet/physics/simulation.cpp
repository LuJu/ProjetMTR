#include "simulation.h"

Simulation::Simulation():
    _world(NULL),
    _simulation_over(false),
    _world_filled(false),
    _initiated(false),
    _end_counter(0),
    _updates_since_last_step(0)
{
}

Simulation::~Simulation(){
    if (_initiated){
        delete _ground;
        delete _world;
        delete _sequentialImpulseConstraintSolver;
        delete _broadphase;
        delete _dispatcher;
        delete _collisionConfiguration;
    }
}

void Simulation::init() {
    SimulationParameters params;
    params.set_gravity(btVector3(0,-9.8,0));
//    params.set_gravity(btVector3(0,0,0));
    params.set_ups(GlobalConfig::get_int("ups"));
    params.set_coefficient(GlobalConfig::get_int("coefficient"));
    params.set_duration(GlobalConfig::get_int("duration") * params.get_coefficient());
    params.set_steps_duration(GlobalConfig::get_int("steps_duration") * params.get_coefficient());
    allocateWorld(params);
    _human.set_mass(GlobalConfig::get_int("body_mass"));
    _human.loadObjects(GlobalConfig::get_string("input_location"));
    _display = _human._parts;
    _constraints = _human._constraints;
    _ground = new InteractiveObject();
    _ground->set_shape(btVector3(3,1,3));
    _ground->set_mass(0); // no gravity
    _ground->get_transform().setOrigin(btVector3(0,-1,0));
    _display.append(_ground);
    _initiated = true;
}

void Simulation::allocateWorld(const SimulationParameters& params){
    _params = params;
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _broadphase = new btDbvtBroadphase();
    _sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

    _world = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_sequentialImpulseConstraintSolver,_collisionConfiguration);
    _world->setGravity(params.get_gravity());
}

void Simulation::start(){
        resetStep();
        _clock.reset();
        _thread = new QThread();
        moveToThread(_thread);
        if (!connect(_thread, SIGNAL(started()), this, SLOT(loop()))) qWarning()<<"Thread cannot be connected";
        qDebug()<<"Starting simulation";
        qDebug()<<"Parameters : \n\tDuration      : "<<_params.get_duration()<<"ms simulation"<<
                               "\n\tSpeed ratio   : 1/"<<_params.get_coefficient()<<"x";
        _thread->start();
}

void Simulation::loop(){
    double time_since_last_step = 0;
    _last_step_time = _clock.getTimeMilliseconds();
    while (_world && !_simulation_over){
        time_since_last_step = _clock.getTimeMilliseconds() - _last_step_time;
        if (time_since_last_step/1000 > 1./(_params.get_ups()*_params.get_coefficient())){
            time_since_last_step = 0;
            _lock.lockForWrite(); {
                update();
//                qDebug()<<time_since_last_step;
                if (_step_counter > _params.get_steps_duration())
                    stepOver();
                if (_end_counter > _params.get_duration())
                    simulationOver();
                _updates_since_last_step++;
            } _lock.unlock();
        }
    }
}

void Simulation::update(){

    _last_step_time = _clock.getTimeMilliseconds();

    btScalar coeff = _params.   get_coefficient();
    btScalar ups = _params.get_ups();
    btScalar progression = 1./(ups*coeff);

    _diff =  _clock.getTimeMicroseconds()/coeff-_elapsed;
    _elapsed=_clock.getTimeMicroseconds()/coeff;

    _world->stepSimulation(progression,0);
    _step_counter+=progression*1000;
    _end_counter+=progression*1000;
    _ups_counter+=progression*1000;
    _human.updateBodyInformations(_elapsed/1000,_diff/1000,_params.get_gravity().y());
}

void Simulation::resetStep(float time){
    cleanWorld();
    _human.setSimulationPosition(time);
    fillWorld();

    qDebug()<<"updates :"<<_updates_since_last_step<<" / ";
    _updates_since_last_step = 0;
    _step_counter = 0;

}

void Simulation::stepOver(){
    _human.calculateWork();
    resetStep(_elapsed/1000);
}

void Simulation::cleanWorld(){
    btRigidBody * body;
    if (_world_filled){
        for (int i = 0; i < _constraints.size(); ++i) {
            _world->removeConstraint(_constraints[i]._constraint);
        }
        btTypedConstraint * auie= _world->getConstraint(0);
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->removeRigidBody(body);
        }
        _world_filled = false; //indicates that the world is now empty
    } else qWarning()<<"Attempting to clean an empty world" ;
}

void Simulation::fillWorld(){
    btRigidBody * body = NULL;
    if (!_world_filled){
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->addRigidBody(body);
        }
        for (int i = 0; i < _constraints.size(); ++i) {
            if (_constraints[i]._constraint == NULL)
                delete _constraints[i]._constraint;
            btPoint2PointConstraint * constraint;
            if (_constraints[i]._parts.second != NULL){
                constraint= new btPoint2PointConstraint(
                    _constraints[i]._parts.first->get_body(),
                    _constraints[i]._parts.second->get_body(),
                    btVector3(0,-_constraints[i]._parts.first->get_shape().y() -0.1,0),
                    btVector3(0,_constraints[i]._parts.first->get_shape().y() +0.1,0));
            } else {
                constraint= new btPoint2PointConstraint(
                    _constraints[i]._parts.first->get_body(),
                    btVector3(0,_constraints[i]._parts.first->get_shape().y() -0.1,0));
            }
                _constraints[i]._constraint = constraint;
//            btPoint2PointConstraint * constraint= new btPoint2PointConstraint((*part1)->get_body(),(*part1)->get_body(),btVector3(0,(*part1)->get_shape().y(),0),btVector3(0,(*part2)->get_shape().y(),0));
//            _constraints.append(joint);
            _world->addConstraint(constraint);
        }
        _world_filled = true;
    } else qWarning()<<"Attempting to fill a full world" ;
}


void Simulation::simulationOver()
{
     _human.saveDataList();
     _simulation_over = true;
     qDebug()<<"\n\nSimulation over";
}
