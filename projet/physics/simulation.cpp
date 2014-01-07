#include "simulation.h"



Simulation::Simulation():
    _initiated(false),
    _going(false),
    _simulation_over(false),
    _timer_autoloop(NULL),
    _timer_simulations(NULL)
{
}

Simulation::~Simulation(){
    if (_initiated){
        deleteData();
    }
}

void Simulation::deleteData(){
    delete _world;
    delete _sequentialImpulseConstraintSolver;
    delete _broadphase;
    delete _dispatcher;
    delete _collisionConfiguration;
    deleteTimer(_timer_autoloop);
    deleteTimer(_timer_simulations);
    deleteTimer(_timer_steps);
    delete _ground;
    _initiated=false;
}

void Simulation::deleteTimer(QTimer * timer){
    if (timer){
        timer->disconnect(timer,SIGNAL(timeout()));
        delete timer;
        timer = NULL;
    }
}



void Simulation::standard() {
    SimulationParameters params;
    params.set_gravity(btVector3(0,-9.8,0));
    params.set_coefficient(GlobalConfig::get_int("coefficient"));
    params.set_duration(GlobalConfig::get_int("duration"));
    params.set_steps_duration(GlobalConfig::get_int("steps_duration"));
    params.set_ups(GlobalConfig::get_int("ups"));
    initiate(params);
    _human.set_mass(GlobalConfig::get_int("body_mass"));
    _human.loadObjects(GlobalConfig::get_string("input_location"));
    _display = _human._parts;
    _ground = new InteractiveObject();
    _ground->set_shape(btVector3(2,0.5,2) );
    _ground->set_mass(0); // no gravity
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

    _initiated=true;
}

void Simulation::startSimulation(){
        _going=true;
        _world_filled = false;
        int coeff = _params.get_coefficient();
        int duration = _params.get_duration();
        _timer_simulations= new QTimer();
        _timer_simulations->setInterval(duration*coeff);
        _timer_simulations->connect(_timer_simulations, SIGNAL(timeout()),this, SLOT(simulationOver()));
        _timer_simulations->start();
        resetStep();

        if (_autoloop == true){
            autoloopStart();
        }
        _clock.reset();

        qDebug()<<"Starting simulation";
        qDebug()<<"Parameters : \n\tduration : "<<_params.get_duration()<<
                               "\n\tCoeff    : "<<_params.get_coefficient();
}

void Simulation::resetStep(float time){
    deleteTimer(_timer_steps);
    _timer_steps= new QTimer();
    _timer_steps->setInterval(_params.get_steps_duration());
    _timer_steps->connect(_timer_steps, SIGNAL(timeout()),this, SLOT(stepOver()));
    _timer_steps->start();

    cleanWorld();
    _elapsed=time;
    _time_beginning = time;
    _human.setSimulationPosition(_elapsed);
    fillWorld();
}

void Simulation::stepOver(){
    _human.calculateWork();
    resetStep(_elapsed);
}

void Simulation::cleanWorld(){
    btRigidBody * body;
    if (_world_filled){
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->removeRigidBody(body);
        }
        _world_filled = false; //indicates that the world is now empty
    } else qWarning()<<"Attempting to clean an empty world" ;
}

void Simulation::fillWorld(){
    //        btVector3 axisA(1.0f, 0.f, 0.f);
    //        btVector3 axisB(1.0f, 0.f, 0.f);
    //        btVector3 pivotA(-1.0f, -1.f, 1.f);
    //        btVector3 pivotB(-1.0f, -1.f, -1.f);
    //        btHingeConstraint* hinge = new btHingeConstraint(*body1, *body2, pivotA, pivotB, axisA, axisB);
    btRigidBody * body;
    if (!_world_filled){
        qDebug()<<_display.size();
        for (int i = 0; i < _display.size(); ++i) {
            body = &(_display[i]->get_body());
            _world->addRigidBody(body);
        }
        _world_filled = true;
    } else qWarning()<<"Attempting to fill a full world" ;
    //    _world->addConstraint(hinge);
}


void Simulation::simulationOver()
{
    if(_going){
         _human.saveDataList();
         _going = false;
         _simulation_over = true;
         qDebug()<<"\n\nSimulation over";
    }
}

void Simulation::autoloopStart(){
    _timer_autoloop= new QTimer();
    _timer_autoloop->setInterval(_params.get_ups());
    _timer_autoloop->connect(_timer_autoloop, SIGNAL(timeout()),this, SLOT(autoloopSteps()));
    _timer_autoloop->start();
}

void Simulation::autoloopSteps(){
    if (_world && _going)
        update();
}

void Simulation::update(){
    btScalar coeff = _params.get_coefficient();
    _diff = _clock.getTimeMilliseconds()/coeff-_elapsed;
    _world->stepSimulation(1./(60.*coeff),0);
    _elapsed=_clock.getTimeMilliseconds()/coeff;
    _human.updateEnergyInformations(_elapsed,_diff,_params.get_gravity().y());
}
