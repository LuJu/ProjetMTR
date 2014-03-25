#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,Shape::shapetype type):
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _collision_shape(NULL){
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _collision_shape(NULL){
    __build(btVector3(0,0,0),btVector3(1,1,1),Shape::cube);
}

void InteractiveObject::appendCurve( int index,QString label, QColor color){
    Curve& c= _curves[index];
    c.set_color(color);
    c.set_label(label);
    _curves.append(c);
}
void InteractiveObject::appendCurveSteps( int index,QString label, QColor color){
    Curve& c= _curves_steps[index];
    c.set_color(color);
    c.set_label(label);
    _curves_steps.append(c);
}

void InteractiveObject::appendCurves(){
    for (int i = 0; i < NUMBER_OF_CURVES; ++i) {
        _curves.append(Curve());
    }
    appendCurve(ANIMATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(ANIMATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(ANIMATION_PE,"Animation KE",QColor(255,255,255));
    appendCurve(SIMULATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(SIMULATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(SIMULATION_PE,"Animation KE",QColor(255,255,25));
    appendCurve(DIFF_KE,"Animation KE",QColor(255,0,255));
    appendCurve(DIFF_AKE,"Animation KE",QColor(0,255,255));
    appendCurve(DIFF_PE,"Animation KE",QColor(255,255,0));
    appendCurve(ANIMATION_Y,"Animation KE",QColor(255,0,255));
    appendCurve(SIMULATION_Y,"Animation KE",QColor(0,255,255));
}

void InteractiveObject::appendCurvesSteps(){
    for (int i = 0; i < NUMBER_OF_CURVES; ++i) {
        _curves_steps.append(Curve());
    }
    appendCurve(ANIMATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(ANIMATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(ANIMATION_PE,"Animation KE",QColor(255,255,255));
    appendCurve(SIMULATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(SIMULATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(SIMULATION_PE,"Animation KE",QColor(255,255,25));
    appendCurve(DIFF_KE,"Animation KE",QColor(255,0,255));
    appendCurve(DIFF_AKE,"Animation KE",QColor(0,255,255));
    appendCurve(DIFF_PE,"Animation KE",QColor(255,255,0));
    appendCurve(ANIMATION_Y,"Animation KE",QColor(255,0,255));
    appendCurve(SIMULATION_Y,"Animation KE",QColor(0,255,255));
}

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);


//    switch(type){
//    case Shape::cube:
//        _shape = new btBoxShape(shape);
//        break;
//    case Shape::cylinder:
//        _shape = new btCylinderShape(shape);
//        break;
//    case Shape::capsule:
//        _shape = new btCapsuleShape(shape.x(),shape.y());
//        break;
//    }
    _transform.setIdentity();
    _transform.setOrigin(origin);

    _animation_from_simulation.get_translation_curves()[0].set_color(QColor(1,0,0));
    _animation_from_simulation.get_translation_curves()[1].set_color(QColor(0,1,0));
    _animation_from_simulation.get_translation_curves()[2].set_color(QColor(0,0,1));

    appendCurves();
    appendCurvesSteps();

    _error_1._speed_error = 0;
    _error_1._ticks= 0;
    _error_2._speed_error = 0;
    _error_2._ticks= 0;

    _energy.ake_diff = 0;
    _energy.ke_diff = 0;
    _energy.length= 0;
    _energy.mean_error = 0;
    _energy.mean_error_2= 0;
    _energy.part_name= "";
    _energy.pe_diff = 0;
    _energy.animation.ake = 0;
    _energy.animation.ke = 0;
    _energy.animation.pe = 0;
    _energy.animation.speed = 0;
    _energy.animation.x = 0;
    _energy.animation.y = 0;
    _energy.animation.z = 0;
    _energy.simulation.ake = 0;
    _energy.simulation.ke = 0;
    _energy.simulation.pe = 0;
    _energy.simulation.speed = 0;
    _energy.simulation.x = 0;
    _energy.simulation.y = 0;
    _energy.simulation.z = 0;
}

//InteractiveObject::InteractiveObject(const InteractiveObject& object) :
//    _animated(false),
//    _mass(object._mass),
//    _transform(object._transform),
//    _local_inertia(object._local_inertia),
//    _body(NULL),
//    _motion_state(NULL){
//    _shape = new btBoxShape(((btBoxShape*)object._shape)->getHalfExtentsWithMargin());
//}

InteractiveObject::~InteractiveObject(){
//    if(_shape) delete _shape;
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
    if(_collision_shape) delete _collision_shape;
}

btRigidBody & InteractiveObject::get_body(){
    if(!_body || !_motion_state){
        buildMotion();
    }
    return *_body;
}

const btRigidBody & InteractiveObject::get_body() const {
    if(!_body || !_motion_state){
        qWarning()<<"body not defined";
    }
    return *_body;
}

void InteractiveObject::deleteMotion(){
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
    if(_collision_shape) delete _collision_shape;
}

void InteractiveObject::buildMotion(){
    deleteMotion();
    _collision_shape = _shape.newCollisionShape();
    _collision_shape->calculateLocalInertia(_mass,_local_inertia);
    _motion_state = new btDefaultMotionState(_transform);
    btRigidBody::btRigidBodyConstructionInfo construction_info(_mass, _motion_state, _collision_shape , _local_inertia);
    _body = new btRigidBody(construction_info);
}

void InteractiveObject::buildMesh(){
    _mesh = MeshPointer(new Mesh);
    _shape.set_shape(btVector3(_animation.scalingVector(0)));
    MeshUtils::addCapsuleShape(_mesh.data(),_shape.get_shape().y(),_shape.get_shape().x());
}

void InteractiveObject::updatePartInfo(float elapsed,float diff,float gravity){



    btVector3 distance(_animation.translationVector(elapsed)-_previous_data._position);
    btVector3 simulation_distance = _body->getCenterOfMassPosition()-_previous_data._position_simulation;
    _animation_speed = distance/(diff/1000); // the diff value is in ms so a conversion is needed to be in m/s


/// Calculated simulation speed
    _calculated_simulation_speed = simulation_distance/(diff/1000);
    _error_1._speed_error += absolute_value(_calculated_simulation_speed.length() - _body->getLinearVelocity().length());
    _previous_data._position = _animation.translationVector(elapsed);
    _previous_data._position_simulation_2 = _previous_data._position_simulation;
    _previous_data._position_simulation = _body->getCenterOfMassPosition();

    btVector3 simulation_distance_2 = _body->getCenterOfMassPosition()-_previous_data._position_simulation_2;
    btVector3 _calculated_simulation_speed_2 = simulation_distance_2 / (2*diff/1000);
    _error_2._speed_error += absolute_value(_calculated_simulation_speed_2.length() - _previous_data._linear_velocity.length());
    _previous_data._linear_velocity = _body->getLinearVelocity();
///

// There are 3 ways of getting the (simulation) speed of the part:
// 1 - getting it from the getLinearVelocity() method of the body class
// 2 - calculating it with the formula s = ( current_position - previous position ) / distance
// 3 - formula s(-1) = ( current_position - previous previous position) / (distance*2)
//    btVector3 speed_simulation =_body->getLinearVelocity(); // first method
    btVector3 speed_simulation =_calculated_simulation_speed; // second method
//    btVector3 speed_simulation =_calculated_simulation_speed_2; // third method

    _angular_speed_animation = (_animation.rotationVector(elapsed) - _previous_data._rotation_animation) / (diff/1000) ;

    _previous_data._rotation_animation = _animation.rotationVector(elapsed);

    updateEnergyStructure(_animation_speed,speed_simulation,gravity,elapsed);
    insertDataToCurves(_curves,elapsed);
}

void InteractiveObject::updateEnergyStructure(btVector3 speed_animation, btVector3 speed_simulation, float gravity, float elapsed){
    _energy.animation.speed = speed_animation.length();
    _energy.simulation.speed = (speed_simulation.length());

    _energy.animation.aspeed = _angular_speed_animation.length();
    _energy.simulation.aspeed = rad2deg(_body->getAngularVelocity().length());

    _energy.animation.ke  = kinetic_energy( speed_animation.length(),_mass);
    _energy.simulation.ke = kinetic_energy( speed_simulation.length(),_mass);

    _energy.animation.pe  = potential_energy(_mass,-gravity,_animation.translationVector(elapsed).y());
    _energy.simulation.pe = potential_energy(_mass,-gravity,_body->getCenterOfMassPosition().y()     );

    _energy.animation.ake = get_angular_EC_animation();
    _energy.simulation.ake = get_angular_EC_simulation();

    _energy.animation.x = _animation.translationVector(elapsed).x();
    _energy.animation.y = _animation.translationVector(elapsed).y();
    _energy.animation.z = _animation.translationVector(elapsed).z();

    _energy.simulation.x = _body->getCenterOfMassPosition().x();
    _energy.simulation.y = _body->getCenterOfMassPosition().y();
    _energy.simulation.z = _body->getCenterOfMassPosition().z();

    _energy.part_name = _body_part_name;

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff = _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff = _energy.animation.pe - _energy.simulation.pe;

    ++_error_1._ticks;
    ++_error_2._ticks;
    _energy.mean_error = get_mean_error();
    _energy.mean_error_2 = get_mean_error_2();

}

void InteractiveObject::insertDataToCurves(QList<Curve>& curves, float elapsed){
//        if (GlobalConfig::is_enabled("display_animation_stats")) {
//            curves[ANIMATION_KE].insert(elapsed,_energy.animation.ke);
//            curves[ANIMATION_AKE].insert(elapsed,_energy.animation.ake);
//            curves[ANIMATION_PE].insert(elapsed,_energy.animation.pe);
//        }
//        if (GlobalConfig::is_enabled("display_simulation_stats")) {
//            curves[SIMULATION_KE].insert(elapsed,_energy.simulation.ke);
//            curves[SIMULATION_AKE].insert(elapsed,_energy.simulation.ake);
//            curves[SIMULATION_PE].insert(elapsed,_energy.simulation.pe);
//        }
//        if (GlobalConfig::is_enabled("display_diff")) {
            curves[DIFF_KE].insert(elapsed,_energy.ke_diff);
            curves[DIFF_AKE].insert(elapsed,_energy.ake_diff);
            curves[DIFF_PE].insert(elapsed,_energy.pe_diff);
            curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
            curves[SIMULATION_Y].insert(elapsed,_energy.simulation.y);
//        }
}

void InteractiveObject::setSimulationTransformFromAnimation(float time){
    btVector3 shape,translation,rotation;
    btQuaternion quat;
    btTransform transform;

    transform.setIdentity();
    rotation = _animation.rotationVector(time);
    quat.setEuler(deg2rad(rotation.y()),
                  deg2rad(rotation.x()),
                  deg2rad(rotation.z()));
    transform.setRotation(quat);

    translation = _animation.translationVector(time);
    transform.setOrigin(translation);
    set_transform(transform);
    shape =btVector3(_animation.scalingVector(time));
    _shape.set_shape(shape);

    _previous_data._position = translation;
    _previous_data._position_simulation = translation;
    _previous_data._position_simulation_2 = translation;
}

void InteractiveObject::setSimulationPosition(float time){

    if (time != 0) insertDataToCurves(_curves_steps,time);

    setSimulationTransformFromAnimation(time);
    buildMotion();
    btRigidBody& body = get_body();
    if (time != 0) {
        buildMotion();
        body.setLinearVelocity(_animation_speed);
        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_animation.x()),
                                          deg2rad(_angular_speed_animation.y()),
                                          deg2rad(_angular_speed_animation.z())));
    } else { // if the simulation is starting

        body.setLinearVelocity(speedAtTime(0));
        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_animation.x()),
                                          deg2rad(_angular_speed_animation.y()),
                                          deg2rad(_angular_speed_animation.z())));
    }

    _previous_data._linear_velocity = _animation_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
    updateAnimationFromSimulationData(time);
}

void InteractiveObject::updateAnimationFromSimulationData(float time){
    btRigidBody& body = get_body();
    _animation_from_simulation.get_translation_curves()[0].insert(time,body.getCenterOfMassPosition().x());
    _animation_from_simulation.get_translation_curves()[1].insert(time,body.getCenterOfMassPosition().y());
    _animation_from_simulation.get_translation_curves()[2].insert(time,body.getCenterOfMassPosition().z());
    _animation_from_simulation.get_rotation_curves()[0].insert(time,rot_x(body.getOrientation()));
    _animation_from_simulation.get_rotation_curves()[1].insert(time,rot_y(body.getOrientation()));
    _animation_from_simulation.get_rotation_curves()[2].insert(time,rot_z(body.getOrientation()));
}

btVector3 InteractiveObject::speedAtTime(float time) const {
    return _animation.translationSlope(time) * 1000.0f;
}

btScalar InteractiveObject::get_moment(){
    btVector3 shape  = _shape.get_shape();
    btScalar m = _mass;
    btScalar R = shape.x();
    btScalar R2 = pow(R,2);
    btScalar h = shape.y();
    btScalar h2 = pow(h,2);

//    qDebug()<<"mass: "<<m;
//    qDebug()<<"mass: "<<R2;
//    qDebug()<<"mass: "<<(m * R2)/2.0f;

    btMatrix3x3 moment_matrix (m*(R2/4.0f + h2/12.0f) , 0               , 0                      ,
                               0                      , (m * R2)/2.0f  , 0                      ,
                               0                      , 0               , m*(R2/4.0f + h2/12.0f) );
    btMatrix3x3 rotation;
    rotation.setRotation(_body->getCenterOfMassTransform().getRotation());
    btMatrix3x3 product = (moment_matrix*rotation);
//        qDebug()<<"matrix moment";
//        qDebug()<<moment_matrix[0][0]<<" "<<moment_matrix[0][1]<<" "<<moment_matrix[0][2];
//        qDebug()<<moment_matrix[1][0]<<" "<<moment_matrix[1][1]<<" "<<moment_matrix[1][2];
//        qDebug()<<moment_matrix[2][0]<<" "<<moment_matrix[2][1]<<" "<<moment_matrix[2][2];
//        qDebug();
//        qDebug()<<"matrix rotations";
//        qDebug()<<rotation[0][0]<<" "<<rotation[0][1]<<" "<<rotation[0][2];
//        qDebug()<<rotation[1][0]<<" "<<rotation[1][1]<<" "<<rotation[1][2];
//        qDebug()<<rotation[2][0]<<" "<<rotation[2][1]<<" "<<rotation[2][2];
//        qDebug();
//        qDebug()<<"product";
//        qDebug()<<product[0][0]<<" "<<product[0][1]<<" "<<product[0][2];
//        qDebug()<<product[1][0]<<" "<<product[1][1]<<" "<<product[1][2];
//        qDebug()<<product[2][0]<<" "<<product[2][1]<<" "<<product[2][2];
//        qDebug();
////    btMatrix3x3 rotation = _body->
        return product[2][2];
}

btScalar InteractiveObject::get_angular_EC_simulation(){
    return pow(_body->getAngularVelocity().length(),2) * get_moment() / 2;

}

btScalar InteractiveObject::get_angular_EC_animation(){
    return pow(deg2rad(_angular_speed_animation.length()),2) * get_moment() / 2;
}

QString InteractiveObject::exportSimulationToAnimation(){
    QString outstring;
    QTextStream output(&outstring);
    QChar c(';');
    QChar nl('\n');
    QChar chars[3] = {'x','y','z'};

    output<<"object"<<c<<_body_part_name<<c<<"display"<<nl;

    output<<"scaling"<<c<<nl;
    output<<"radius"<<c<<0.0<<c<<_shape.get_shape().x()<<c<<nl;
    output<<"length"<<c<<0.0<<c<<_shape.get_shape().y()<<c<<nl;

    output<<"translation"<<c<<nl;
    for (int j = 0; j < 3; ++j) {
        Curve curve = _animation_from_simulation.get_translation_curves()[j];
        output<<chars[j]<<c;
        for (int i = 0; i < curve.size(); ++i) {
            output<<curve.keys()[i]<<c<<curve.value(curve.keys()[i])<<c;
        }
        output<<nl;
    }
    output<<"rotation"<<c<<nl;
    for (int j = 0; j < 3; ++j) {
        Curve curve = _animation_from_simulation.get_rotation_curves()[j];
        output<<chars[j]<<c;
        for (int i = 0; i < curve.size(); ++i) {
            output<<curve.keys()[i]<<c<<curve.value(curve.keys()[i])<<c;
        }
        output<<nl;
    }
    output<<"end"<<nl;
    return outstring;
}
