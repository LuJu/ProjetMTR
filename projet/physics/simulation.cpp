#include "simulation.h"

Simulation::Simulation():
    _world(NULL),
    _simulation_over(false),
    _world_filled(false),
    _initiated(false),
    _end_counter(0),
    _updates_since_last_step(0),
    _started(false),
    _elapsed_simulation(0)
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

void Simulation::init(const SimulationParameters& params) {
    _params = params;
    allocateWorld();
    _initiated = true;
    _human.set_mass(params.get_body_mass());
    _human.loadObjects(params.get_input_location());
    _display = _human._parts;
    _joints = _human._constraints;
    _ground = new InteractiveObject();
    _ground->get_shape_struct().set_shape(btVector3(3,1,3));
    _ground->set_mass(0); // no gravity
    _ground->get_transform().setOrigin(btVector3(0,-3,0));

//    _ground->get_transform().setRotation(btQuaternion(btVector3(0,0.0,1),23));
    _ground->get_shape_struct().set_shape_type(Shape::cube);

    _display.append(_ground);
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
        resetStep();
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

void Simulation::update(){

    btScalar coeff = _params.get_coefficient();
    btScalar ups = _params.get_ups();
    btScalar progression_s = 1.d/(ups * coeff);
    btScalar progression_ms = 1000.0d*(1./(ups * coeff));
    btScalar clock_time = _clock.getTimeMicroseconds()/coeff;

    _diff =  clock_time-_elapsed_realtime;
    _elapsed_realtime=clock_time;
    _elapsed_simulation=_elapsed_simulation+progression_ms;
//    InteractiveObject * obj = _display.at(0);
//    obj->get_body().setLinearVelocity(btVector3(0,0,0));
//    qDebug()<<"1: "<<obj->get_body().getCenterOfMassPosition().x()<<" "
//              <<obj->get_body().getCenterOfMassPosition().y()<<" "
//                <<obj->get_body().getCenterOfMassPosition().z()<<" ";
//    qDebug()<<"velocity: "<<obj->get_body().getLinearVelocity().x()<<" "
//           <<obj->get_body().getLinearVelocity().y()<<" "
//          <<obj->get_body().getLinearVelocity().z()<<" ";
    _world->stepSimulation(progression_s ,1,btScalar(1.0/(ups)));
//    qDebug()<<"2: "<<obj->get_body().getCenterOfMassPosition().x()<<" "
//           <<obj->get_body().getCenterOfMassPosition().y()<<" "
//          <<obj->get_body().getCenterOfMassPosition().z()<<" ";
//    qDebug()<<"velocity: "<<obj->get_body().getLinearVelocity().x()<<" "
//           <<obj->get_body().getLinearVelocity().y()<<" "
//          <<obj->get_body().getLinearVelocity().z()<<" ";
    _step_counter+=progression_ms; // conversion from seconds to ms
    _end_counter+=progression_ms;
    _ups_counter+=progression_ms;
    _human.updateBodyInformations(_elapsed_simulation,progression_ms,_params.get_gravity().y());
}

void Simulation::resetStep(float time){
    cleanWorld();
    _human.setSimulationPosition(time);
    fillWorld();
//    btVector3 position = _joints.at(0).get_world_position_simulation();
//    qDebug()<<"updates :"<<_updates_since_last_step<<" / ";
    _updates_since_last_step = 0;
    _step_counter = 0;
}

void Simulation::stepOver(){
    _human.recordStatus();
//    qDebug()<<"time step over: "<<_clock.getTimeMilliseconds();
    resetStep(_elapsed_simulation);
}

void Simulation::cleanWorld(){
    btRigidBody * body;
    if (_world_filled){
        for (int i = 0; i < _joints.size(); ++i) {
            _world->removeConstraint(_joints[i].get_constraint());
        }
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
        for (int i = 0; i < _joints.size(); ++i) {
            _joints[i].buildConstraint();
//            _world->addConstraint(_joints[i].get_constraint(),true);
        }
        _world_filled = true;
    } else qWarning()<<"Attempting to fill a full world" ;
}


void Simulation::simulationOver()
{
     _human.saveDataList();
     _human.saveFullDataList(_params);
     _human.saveCompleteDataList();
     _human.exportSimulationToAnimation();
     _simulation_over = true;
     qDebug()<<"\n\nSimulation over";
     if (_params.get_automatic_close())
        QApplication::exit();
}
