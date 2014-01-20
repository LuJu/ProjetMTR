#include "simulation.h"

Simulation::Simulation():
    _simulation_over(false),
    _end_counter(0),
    _updates_since_last_step(0)
{
}

Simulation::~Simulation(){
    delete _world;
    delete _sequentialImpulseConstraintSolver;
    delete _broadphase;
    delete _dispatcher;
    delete _collisionConfiguration;
    delete _ground;
}

void Simulation::standard() {
    SimulationParameters params;
    params.set_gravity(btVector3(0,-9.8,0));
//    params.set_gravity(btVector3(0,0,0));
    params.set_ups(GlobalConfig::get_int("ups"));
    params.set_coefficient(GlobalConfig::get_int("coefficient"));
    params.set_duration(GlobalConfig::get_int("duration") * params.get_coefficient());
    params.set_steps_duration(GlobalConfig::get_int("steps_duration") * params.get_coefficient());
    initiate(params);
    _human.set_mass(GlobalConfig::get_int("body_mass"));
    _human.loadObjects(GlobalConfig::get_string("input_location"));
    _display = _human._parts;
    _constraints = _human._constraints;
    _ground = new InteractiveObject();
    _ground->set_shape(btVector3(3,1,3));
    _ground->set_mass(0); // no gravity
    _ground->get_transform().setOrigin(btVector3(0,-1,0));
    _display.append(_ground);
}

void Simulation::initiate(const SimulationParameters& params){
    _params = params;
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    _broadphase = new btDbvtBroadphase();
    _sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;

    _world = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_sequentialImpulseConstraintSolver,_collisionConfiguration);
    _world->setGravity(params.get_gravity());
}

void Simulation::threadSystem(){
    _thread = new QThread();
    moveToThread(_thread);
    if (!connect(_thread, SIGNAL(started()), this, SLOT(update()))) qWarning()<<"Thread cannot be connected";
    _thread->start();
}

void Simulation::startSimulation(){
        _world_filled = false;
        int duration = _params.get_duration();
        resetStep();
        threadSystem();
        _clock.reset();

        qDebug()<<"Starting simulation";
        qDebug()<<"Parameters : \n\tDuration      : "<<_params.get_duration()<<"ms simulation"<<
                               "\n\tSpeed ratio   : 1/"<<_params.get_coefficient()<<"x";
}

void Simulation::update(){
    double time_since_last_step = 0;
    _last_step_time = _clock.getTimeMilliseconds();
    while (_world && !_simulation_over){
        time_since_last_step = _clock.getTimeMilliseconds() - _last_step_time;
        if (time_since_last_step/1000 > 1./_params.get_ups()*_params.get_coefficient()){
            _mutex.lock(); {
                time_since_last_step = 0;
                _last_step_time = _clock.getTimeMilliseconds();

                btScalar coeff = _params.get_coefficient();
                btScalar ups = _params.get_ups();
                btScalar progression = 1./(ups*coeff);

                _diff =  _clock.getTimeMicroseconds()/coeff-_elapsed;
                _elapsed=_clock.getTimeMicroseconds()/coeff;

                _world->stepSimulation(progression,0);
                _step_counter+=progression*1000;
                _end_counter+=progression*1000;
                _ups_counter+=progression*1000;
            //    _world->stepSimulation(1./60.0,0);
                _human.updateBodyInformations(_elapsed/1000,_diff/1000,_params.get_gravity().y());

                if (_step_counter > _params.get_steps_duration())
                    stepOver();
                if (_end_counter > _params.get_duration())
                    simulationOver();
            //    if (_ups_counter > })
                _updates_since_last_step++;
            } _mutex.unlock();
        }
    }
}

void Simulation::resetStep(float time){
    cleanWorld();
//    _time_beginning = time;
    qDebug()<<"updates :"<<_updates_since_last_step<<" / ";

    _updates_since_last_step = 0;
    _human.setSimulationPosition(time);
    fillWorld();

    _step_counter = 0;

}

void Simulation::stepOver(){
    _human.calculateWork();
    resetStep(_elapsed/1000);
}

void Simulation::cleanWorld(){
    btRigidBody * body;
    if (_world_filled){
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->removeRigidBody(body);
        }
        for (int i = 0; i < _constraints.size(); ++i) {
            _world->removeConstraint(_constraints[i]);
        }
        _world_filled = false; //indicates that the world is now empty
    } else qWarning()<<"Attempting to clean an empty world" ;
}

void Simulation::fillWorld(){
    btRigidBody * body;
    if (!_world_filled){
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->addRigidBody(body);
        }
        for (int i = 0; i < _constraints.size(); ++i) {
            btPoint2PointConstraint * constraint= new btPoint2PointConstraint ((_display[0]->get_body()),(_display[1]->get_body()),btVector3(0,1,0),btVector3(1,0,0));
//            _world->addConstraint(_constraints[i]);
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
