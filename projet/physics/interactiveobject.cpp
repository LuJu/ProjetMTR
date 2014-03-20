#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,shapetype type):
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(type){
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _animated(false),
    _body(NULL),
    _motion_state(NULL),
    _local_inertia(btVector3(0,0,0)),
    _shape_type(cube){
    __build(btVector3(0,0,0),btVector3(1,1,1),cube);
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

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,shapetype type){
    switch(type){
    case cube:
        _shape = new btBoxShape(shape);
        break;
    case cylinder:
        _shape = new btCylinderShape(shape);
        break;
    case capsule:
        _shape = new btCapsuleShape(shape.x(),shape.y());
        break;
    }
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
    if(_shape) delete _shape;
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
}

btRigidBody & InteractiveObject::get_body(){
    if(!_body || !_motion_state){
        buildMotion();
    }
    return *_body;
}

void InteractiveObject::deleteMotion(){
    if(_body)  delete _body;
    if(_motion_state) delete _motion_state;
}

void InteractiveObject::buildMotion(){
    deleteMotion();
    _shape->calculateLocalInertia(_mass,_local_inertia);
    _motion_state = new btDefaultMotionState(_transform);
    btRigidBody::btRigidBodyConstructionInfo construction_info(_mass, _motion_state, _shape, _local_inertia);
    _body = new btRigidBody(construction_info);
    ;
}

void InteractiveObject::buildMesh(){
    mesh = MeshPointer(new Mesh);
//    btVector3(_animation.scalingVector(0));
    set_shape(btVector3(_animation.scalingVector(0)));
    MeshUtils::addCapsuleShape(mesh.data(),get_shape().y(),get_shape().x());
    qDebug()<<"shape : "<<get_shape().x()<<" "<<get_shape().y();
}

btVector3 InteractiveObject::get_shape() const {
    switch (_shape_type){
    case cube:
        return ((btBoxShape *) _shape)->getHalfExtentsWithMargin();
        break;
    case cylinder:
        return ((btCylinderShape *) _shape)->getHalfExtentsWithMargin();
        break;
    case capsule:
        btVector3 shape;
        shape.setX(((btCapsuleShape *)_shape)->getRadius());
        shape.setY(((btCapsuleShape *)_shape)->getHalfHeight()*2);
        shape.setZ(((btCapsuleShape *)_shape)->getRadius());
        return shape;
        break;
    }
}

void InteractiveObject::set_shape(const btVector3 &shape){
    delete _shape;
    switch (_shape_type){
    case cube:
        _shape = new btBoxShape(shape);
        break;
    case cylinder:
        _shape = new btCylinderShape(shape);
        break;
    case capsule:
        _shape = new btCapsuleShape(shape.x(),shape.y());
        break;
    }

}

const InteractiveObject::t_part_info& InteractiveObject::updatePartInfo(float elapsed,float diff,float gravity){
    btVector3 distance(_animation.translationVector(elapsed)-_previous_data._position);
    btVector3 simulation_distance = _body->getCenterOfMassPosition()-_previous_data._position_simulation;
    btVector3 speed_animation(distance/(diff/1000)); // the diff value is in ms so a conversion is needed to be in m/s
    _animation_speed = speed_animation;


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
//    qDebug()<<"angular speed : "<<_angular_speed_rotation.x()<<" "<<_angular_speed_rotation.y()<<" "<<_angular_speed_rotation.z();
//    qDebug()<<"rotation :";
//    qDebug()<<_animation.rotationVector(elapsed).x();
//    qDebug()<<_animation.rotationVector(elapsed).y();
//    qDebug()<<_animation.rotationVector(elapsed).z();
//    qDebug();
//    qDebug()<<_previous_data._rotation_animation.x();
//    qDebug()<<_previous_data._rotation_animation.y();
//    qDebug()<<_previous_data._rotation_animation.z();
//    qDebug();
//    qDebug()<<_angular_speed_rotation.x();
//              qDebug()<<_angular_speed_rotation.y();
//              qDebug()<<_angular_speed_rotation.z();

    _previous_data._rotation_animation = _animation.rotationVector(elapsed);
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

//    _animation_from_simulation.get_translation_curves()[0].insert(elapsed,_body->getCenterOfMassPosition().x());
//    _animation_from_simulation.get_translation_curves()[1].insert(elapsed,_body->getCenterOfMassPosition().y());
//    _animation_from_simulation.get_translation_curves()[2].insert(elapsed,_body->getCenterOfMassPosition().z());
//    _animation_from_simulation.get_rotation_curves()[0].insert(elapsed,rot_x(_body->getOrientation()));
//    _animation_from_simulation.get_rotation_curves()[1].insert(elapsed,rot_y(_body->getOrientation()));
//    _animation_from_simulation.get_rotation_curves()[2].insert(elapsed,rot_z(_body->getOrientation()));

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff = _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff = _energy.animation.pe - _energy.simulation.pe;

    ++_error_1._ticks;
    ++_error_2._ticks;
    _energy.mean_error = get_mean_error();
    _energy.mean_error_2 = get_mean_error_2();

//    if (GlobalConfig::is_enabled("display_speed"))
//        _curves[0].insert(elapsed,_energy.simulation.speed);
//    if (GlobalConfig::is_enabled("display_error"))
//        _curves[3].insert(elapsed,_energy.mean_error);
//    if (GlobalConfig::is_enabled("display_animation_stats")) {
//        _curves[0].insert(elapsed,_energy.animation.ke);
//        _curves[1].insert(elapsed,_energy.animation.ake);
        _curves[2].insert(elapsed,_energy.animation.pe);
//    }
//    if (GlobalConfig::is_enabled("display_simulation_stats")) {
//        _curves[3].insert(elapsed,_energy.simulation.ke);
//        _curves[4].insert(elapsed,_energy.simulation.ake);
        _curves[5].insert(elapsed,_energy.simulation.pe);
//    }
//    if (GlobalConfig::is_enabled("display_diff")) {
//        _curves[6].insert(elapsed,_energy.ke_diff);
//        _curves[7].insert(elapsed,_energy.ake_diff);
        _curves[DIFF_PE].insert(elapsed,_energy.pe_diff);
        _curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
        _curves[SIMULATION_Y].insert(elapsed,_energy.simulation.y);
//    }
    return _energy;
}

void InteractiveObject::setSimulationPosition(float time){

//    if (GlobalConfig::is_enabled("display_animation_stats")) {
//        _curves_steps[0].insert(time,_energy.animation.ke);
//        _curves_steps[1].insert(time,_energy.animation.ake);
        _curves_steps[ANIMATION_PE].insert(time,_energy.animation.pe);
//    }
//    if (GlobalConfig::is_enabled("display_simulation_stats")) {
//        _curves_steps[3].insert(time,_energy.simulation.ke);
//        _curves_steps[4].insert(time,_energy.simulation.ake);
        _curves_steps[SIMULATION_PE].insert(time,_energy.simulation.pe);
//    }
//    if (GlobalConfig::is_enabled("display_diff")) {
//        _curves_steps[6].insert(time,_energy.ke_diff);
//        _curves_steps[7].insert(time,_energy.ake_diff);
        _curves_steps[DIFF_PE].insert(time,_energy.pe_diff);
//    }


    btVector3 shape,translation,rotation;
    btQuaternion quat;
    btTransform transform;

    transform.setIdentity();

    rotation = _animation.rotationVector(time);
    quat.setEuler(deg2rad(rotation.x()),
                  deg2rad(rotation.y()),
                  deg2rad(rotation.z()));
//    quat.setEuler(deg2rad(rotation.x()),
//                  deg2rad(rotation.y()),
//                  deg2rad(rotation.z()));
    transform.setRotation(quat);

    translation = _animation.translationVector(time);
    transform.setOrigin(translation);
    set_transform(transform);
    shape =btVector3(_animation.scalingVector(time));
    set_shape(shape);
    buildMotion();
    if (time != 0) {
        btRigidBody& body = get_body();
        body.setLinearVelocity(_animation_speed);
//        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_rotation.x()),
//                                          deg2rad(_angular_speed_rotation.y()),
//                                          deg2rad(_angular_speed_rotation.z())));
        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_animation.y()),
                                          deg2rad(_angular_speed_animation.x()),
                                          deg2rad(_angular_speed_animation.z())));
//        qDebug()<<" rotation "<<_angular_speed_animation.length();
        _previous_data._linear_velocity = _animation_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
    } else { // if the simulation is starting
        btRigidBody& body = get_body();
        _animation_from_simulation.get_translation_curves()[0].insert(time,_body->getCenterOfMassPosition().x());
        _animation_from_simulation.get_translation_curves()[1].insert(time,_body->getCenterOfMassPosition().y());
        _animation_from_simulation.get_translation_curves()[2].insert(time,_body->getCenterOfMassPosition().z());
        _animation_from_simulation.get_rotation_curves()[0].insert(time,rot_x(_body->getOrientation()));
        _animation_from_simulation.get_rotation_curves()[1].insert(time,rot_y(_body->getOrientation()));
        _animation_from_simulation.get_rotation_curves()[2].insert(time,rot_z(_body->getOrientation()));

        body.setLinearVelocity(speedAtTime(0));
//        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_rotation.x()),
//                                          deg2rad(_angular_speed_rotation.y()),
//                                          deg2rad(_angular_speed_rotation.z())));
        body.setAngularVelocity(btVector3(deg2rad(_angular_speed_animation.y()),
                                          deg2rad(_angular_speed_animation.x()),
                                          deg2rad(_angular_speed_animation.z())));
//        qDebug()<<" rotation "<<_angular_speed_rotation.length();
        _previous_data._linear_velocity = _animation_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
    }

    _previous_data._position = translation;
    _previous_data._position_simulation = translation;
    _previous_data._position_simulation_2 = translation;

}

btVector3 InteractiveObject::speedAtTime(float time) {
    btVector3 animation_speed;
    _animation.get_translation_curves();
    btVector3 init_position = _animation.translationVector(0);
    if (_animation.get_translation_curves()[0].size()>1){
        btScalar diff = _animation.get_translation_curves()[0].keys()[1];
        btVector3 second = _animation.translationVector(diff);
        btVector3 distance(second-init_position);
        btVector3 speed_animation(distance/(diff/1000)); // the diff value is in ms so a conversion is needed to be in m/s
        animation_speed = speed_animation;
    } else
        animation_speed = btVector3(0.0,0.0,0.0);
    return animation_speed;
}

btScalar InteractiveObject::get_volume(){
    // for capsule : volume of cylinder of radius R and height H + volume of sphere of radius R
    // = pi.R^2.h + 4/3.pi.R^3
    btScalar r = get_shape().x();
    btScalar h = get_shape().y();
    btScalar sphere_volume = (4/3) * M_PI * pow(r,3);
    btScalar cylinder_volume =  M_PI * pow(r,2) * h;
    return cylinder_volume+sphere_volume;
}



//thin cylinder (mass m , length h) rotating with the axis on one of the extremities
// = 1/3.m.h
btScalar InteractiveObject::get_moment(){
    btScalar moment = pow(get_shape().y(),2)*
            _mass/3;
//    qDebug()<<"moment : "<<moment;
    btScalar m = _mass;
    btScalar R = get_shape().x();
    btScalar R2 = pow(R,2);
    btScalar h = get_shape().y();
    btScalar h2 = pow(h,2);

    qDebug()<<"mass: "<<m;
    qDebug()<<"mass: "<<R2;
    qDebug()<<"mass: "<<(m * R2)/2.0f;

    btMatrix3x3 moment_matrix (m*(R2/4.0f + h2/12.0f) , 0               , 0                      ,
                               0                      , (m * R2)/2.0f  , 0                      ,
                               0                      , 0               , m*(R2/4.0f + h2/12.0f) );
    btMatrix3x3 rotation;
    rotation.setRotation(_body->getCenterOfMassTransform().getRotation());
    btMatrix3x3 product = (moment_matrix*rotation);
        qDebug()<<"matrix moment";
        qDebug()<<moment_matrix[0][0]<<" "<<moment_matrix[0][1]<<" "<<moment_matrix[0][2];
        qDebug()<<moment_matrix[1][0]<<" "<<moment_matrix[1][1]<<" "<<moment_matrix[1][2];
        qDebug()<<moment_matrix[2][0]<<" "<<moment_matrix[2][1]<<" "<<moment_matrix[2][2];
        qDebug();
        qDebug()<<"matrix rotations";
        qDebug()<<rotation[0][0]<<" "<<rotation[0][1]<<" "<<rotation[0][2];
        qDebug()<<rotation[1][0]<<" "<<rotation[1][1]<<" "<<rotation[1][2];
        qDebug()<<rotation[2][0]<<" "<<rotation[2][1]<<" "<<rotation[2][2];
        qDebug();
        qDebug()<<"product";
        qDebug()<<product[0][0]<<" "<<product[0][1]<<" "<<product[0][2];
        qDebug()<<product[1][0]<<" "<<product[1][1]<<" "<<product[1][2];
        qDebug()<<product[2][0]<<" "<<product[2][1]<<" "<<product[2][2];
        qDebug();
//    btMatrix3x3 rotation = _body->
        return product[2][2];
}

btScalar InteractiveObject::get_angular_EC_simulation(){
//    qDebug()<<"simulation velocity: "<<rad2deg(_body->getAngularVelocity().length());
    return _body->getAngularVelocity().length() * _body->getAngularVelocity().length() * get_moment() / 2;

}

btScalar InteractiveObject::get_angular_EC_animation(){
//    qDebug()<<"animation velocity: "<<_angular_speed_rotation.length();
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
    output<<"radius"<<c<<0.0<<c<<get_shape().x()<<c<<nl;
    output<<"length"<<c<<0.0<<c<<get_shape().y()<<c<<nl;

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

float InteractiveObject::rot_x(btQuaternion q) const
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float rotx = atan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
    return rad2deg(rotx);
}

float InteractiveObject::rot_y(btQuaternion q) const
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float roty = asin(2 * ((m_w * m_y) - (m_z * m_x)));
    return rad2deg(roty);
}


float InteractiveObject::rot_z(btQuaternion q) const
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float rotz = atan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
    return rad2deg(rotz);
}

btVector3 InteractiveObject::btQuat2euler(btQuaternion q){
    return btVector3(rot_x(q),rot_y(q),rot_z(q));
}
