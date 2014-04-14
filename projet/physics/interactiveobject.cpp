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

void InteractiveObject::appendCurve(QList<Curve>& list, int index,QString label, QColor color){
    Curve& c= list[index];
    c.set_color(color);
    c.set_label(label);
}

void InteractiveObject::appendCurves(QList<Curve>& list){
    for (int i = 0; i < NUMBER_OF_CURVES; ++i) {
        list.append(Curve());
    }
    appendCurve(list,ANIMATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(list,ANIMATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(list,ANIMATION_PE,"Animation KE",QColor(255,255,255));
    appendCurve(list,SIMULATION_KE,"Animation KE",QColor(255,0,0));
    appendCurve(list,SIMULATION_AKE,"Animation KE",QColor(0,0,255));
    appendCurve(list,SIMULATION_PE,"Animation KE",QColor(255,255,25));
    appendCurve(list,DIFF_KE,"Animation KE",QColor(255,0,255));
    appendCurve(list,DIFF_AKE,"Animation KE",QColor(0,255,255));
    appendCurve(list,DIFF_PE,"Animation KE",QColor(255,255,0));
    appendCurve(list,ANIMATION_X,"Animation KE",QColor(255,0,0));
    appendCurve(list,ANIMATION_Y,"Animation KE",QColor(255,0,255));
    appendCurve(list,SIMULATION_Y,"Animation KE",QColor(0,255,255));
}

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);

    _transform.setIdentity();
    _transform.setOrigin(origin);

    _animation_from_simulation.get_translation_curves()[0].set_color(QColor(1,0,0));
    _animation_from_simulation.get_translation_curves()[1].set_color(QColor(0,1,0));
    _animation_from_simulation.get_translation_curves()[2].set_color(QColor(0,0,1));

    appendCurves(_curves);
    appendCurves(_curves_steps);

    _energy.ake_diff = 0;
    _energy.ke_diff = 0;
    _energy.length= 0;
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

InteractiveObject::~InteractiveObject(){
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

void InteractiveObject::updatePartInfo(float elapsed,float delta_t,float gravity,const btTransform& transform){

    updateAnimation(elapsed,delta_t,gravity,transform);
    updateSimulation(elapsed,delta_t,gravity);

    updateEnergyStructure(gravity);
    insertDataToCurves(_curves,elapsed);
    _animation._previous_state = _animation._current_state;
    _simulation._previous_state = _simulation._current_state;
}

void InteractiveObject::updateAnimation(float elapsed,float delta_t,float gravity,const btTransform& transform){
    t_state_data& current = _animation._current_state;
    t_state_data& previous =_animation._previous_state;
    current._position = transform.getOrigin();
//    current._position = _animation.translationVector(elapsed);
    //    current._rotation = _animation.rotationVector(elapsed);
    current._rotation = btQuat2euler(transform.getRotation());

    btVector3 animation_distance(current._position-previous._position);

    current._center_of_mass_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s
    current._angular_speed = (current._rotation - previous._rotation) / (delta_t/1000) ;
    current._rotation_vector_diff = (current._rotation -previous._rotation).normalized();
}

void InteractiveObject::updateSimulation(float elapsed,float delta_t,float gravity){
    t_state_data& current = _simulation._current_state;
    t_state_data& previous =_simulation._previous_state;
    current._position = _body->getCenterOfMassPosition();

    btVector3 simulation_distance = current._position-previous._position;
    btQuaternion rot = _body->getCenterOfMassTransform().getRotation();
    _calculated_simulation_speed = simulation_distance/(delta_t/1000);

    current._angular_speed= _body->getAngularVelocity();
    current._center_of_mass_speed = _body->getLinearVelocity();
    current._rotation = btQuat2euler(rot);
    current._rotation_vector_diff = (current._rotation -previous._rotation).normalized();
}

void InteractiveObject::updateEnergyStructure(float gravity){

    _energy.part_name = _body_part_name;

    _energy.animation.x = _animation._current_state._position.x();
    _energy.animation.y = _animation._current_state._position.y();
    _energy.animation.z = _animation._current_state._position.z();

    _energy.simulation.x = _simulation._current_state._position.x();
    _energy.simulation.y = _simulation._current_state._position.y();
    _energy.simulation.z = _simulation._current_state._position.z();

    _energy.animation.speed = _animation._current_state._center_of_mass_speed.length();
    _energy.simulation.speed = (_simulation._current_state._center_of_mass_speed.length());

    _energy.animation.aspeed = _animation._current_state._angular_speed.length();
    _energy.animation.pt_aspeed.x = _animation._current_state._angular_speed.x();
    _energy.animation.pt_aspeed.y = _animation._current_state._angular_speed.y();
    _energy.animation.pt_aspeed.z = _animation._current_state._angular_speed.z();

    _energy.simulation.aspeed =      rad2deg(_simulation._current_state._angular_speed.length());
    _energy.simulation.pt_aspeed.x = rad2deg(_simulation._current_state._angular_speed.x());
    _energy.simulation.pt_aspeed.y = rad2deg(_simulation._current_state._angular_speed.y());
    _energy.simulation.pt_aspeed.z = rad2deg(_simulation._current_state._angular_speed.z());

    _energy.animation.ke  = kinetic_energy( _animation._current_state._center_of_mass_speed.length(),_mass);
    _energy.simulation.ke = kinetic_energy( _simulation._current_state._center_of_mass_speed.length(),_mass);

    _energy.animation.pe  = potential_energy(_mass,-gravity,_animation._current_state._position.y());
    _energy.simulation.pe = potential_energy(_mass,-gravity,_simulation._current_state._position.y());

    _energy.animation.ake = get_angular_EC_animation();
    _energy.simulation.ake = get_angular_EC_simulation();

    _energy.ake_diff = _energy.animation.ake - _energy.simulation.ake;
    _energy.ke_diff =  _energy.animation.ke - _energy.simulation.ke;
    _energy.pe_diff =  _energy.animation.pe - _energy.simulation.pe;

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
//            curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
//            curves[ANIMATION_X].insert(elapsed,_energy.animation.x);
//            curves[ANIMATION_Y].insert(elapsed,_energy.animation.y);
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
    _simulation._previous_state._position = translation;
}

void InteractiveObject::setSimulationPosition(float time){

    if (time != 0) insertDataToCurves(_curves_steps,time);

    setSimulationTransformFromAnimation(time);
    buildMotion();
    btRigidBody& body = get_body();
    if (time != 0) {
        buildMotion();
        body.setLinearVelocity(_animation._current_state._center_of_mass_speed);
        body.setAngularVelocity(btVector3(deg2rad(_animation._current_state._angular_speed.x()),
                                          deg2rad(_animation._current_state._angular_speed.y()),
                                          deg2rad(_animation._current_state._angular_speed.z())));
    } else { // if the simulation is starting

        _animation._current_state._center_of_mass_speed = speedAtTime(0.0f);
        _animation._current_state._angular_speed = _animation.rotationTangent(time) ;
        body.setLinearVelocity(_animation._current_state._center_of_mass_speed);
        body.setAngularVelocity(btVector3(deg2rad(_animation._current_state._angular_speed.x()),
                                          deg2rad(_animation._current_state._angular_speed.y()),
                                          deg2rad(_animation._current_state._angular_speed.z())));
        _animation._previous_state._position = _animation.translationVector(0.0f);
        _simulation._previous_state._position = _body->getCenterOfMassPosition();
//        _simulation._previous_state._position_2 = _animation._previous_state._position;
        _simulation._previous_state._center_of_mass_speed = _body->getLinearVelocity();
        _animation._previous_state._rotation = _animation.rotationVector(0.0f);
        _calculated_simulation_speed = _animation._current_state._center_of_mass_speed;
    }

    _simulation._previous_state._center_of_mass_speed = _animation._current_state._center_of_mass_speed; // sets the previous speed to the same as currens speed to avoid calculation errors
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
    return _animation.translationTangent(time) * 1000.0f;
}

btScalar InteractiveObject::get_angular_EC_simulation(){
    return pow(_body->getAngularVelocity().length(),2) * get_moment(_simulation._current_state._rotation_vector_diff,_shape.get_shape(),_mass) / 2;

}

btScalar InteractiveObject::get_angular_EC_animation(){
    return pow(deg2rad(_animation._current_state._angular_speed.length()),2) * get_moment(_animation._current_state._rotation_vector_diff,_shape.get_shape(),_mass) / 2;
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

btVector3 InteractiveObject::get_extremity_animation  ()const{
    btVector3 center = _animation._current_state._position;
    btVector3 ypr = _animation._current_state._rotation;

    btScalar R = _shape.get_shape().y()/2;
    btQuaternion quat;

    quat.setEuler(deg2rad(ypr.y()),deg2rad(ypr.x()),deg2rad(ypr.z()));
    QVector3D top_position(0,R,0);
    quat.normalize();
    QQuaternion qquat(quat.w(),quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z());
    qquat= qquat.fromAxisAndAngle(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z(),rad2deg(quat.getAngle()));

    top_position = qquat.rotatedVector(top_position);

    return center + btVector3(top_position.x(),top_position.y(),top_position.z());
}

btVector3 InteractiveObject::get_center_position () const {


}
