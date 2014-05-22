#include "interactiveobject.h"

InteractiveObject::InteractiveObject(const btVector3& origin, const btVector3& shape,Shape::shapetype type):
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    qDebug()<<"Not default constructor called";
    __build(origin,shape,type);
}

InteractiveObject::InteractiveObject():
    _mass(1),
    _center_of_mass_proportion(.5f),
    _animated(false){
    __build(btVector3(0,0,0),btVector3(1,1,1),Shape::cube);
}

void InteractiveObject::appendCurve(QList<Curve>& list, int index,QString label, QColor color){
    Curve& c= list[index];
    c.set_color(color);
    c.set_label(label);
}

InteractiveObject::~InteractiveObject(){
}

void InteractiveObject::__build(const btVector3& origin, const btVector3& shape,Shape::shapetype type){
    _shape = Shape(shape,type);
    _simulation.set_shape (&_shape);

    _original_transform.setIdentity();
    _original_transform.setOrigin(origin);

    appendCurves(_curves);
    appendCurves(_curves_steps);
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



void InteractiveObject::deleteMotion(){
}

void InteractiveObject::buildMotion(){
    _simulation.buildMotion(_mass,_original_transform,_center_of_mass_proportion);
}

void InteractiveObject::buildMesh(){
    _mesh = MeshPointer(new Mesh);
    MeshUtils::addCapsuleShapeY(_mesh.data(),_shape.get_shape().y(),_shape.get_shape().x());
}

void InteractiveObject::updatePartInfo(float elapsed,float delta_t,float gravity,const btTransform& transform, const btTransform& parent_transform){

    _animation_information._previous = _animation_information._current;
    _simulation_information._previous = _simulation_information._current;

    updateAnimation(delta_t,transform,parent_transform);
    updateSimulation(delta_t);

    updateEnergyStructure(gravity);
    insertDataToCurves(_curves,elapsed);
}

btScalar InteractiveObject::angleDifference(btVector3 v1, btVector3 v2){
    btScalar dot_product = v1.dot(v2);
    btScalar cost = dot_product / (v2.length() * v1.length());
    btScalar rotation_angle = qAcos(cost);
    return rotation_angle;
}

void InteractiveObject::updateAnimation(float delta_t,const btTransform& transform, const btTransform& parent_transform){
    t_state_data& current = _animation_information._current;
    t_state_data& previous =_animation_information._previous;

    btScalar rotation_angle;
    btVector3 rotation_axis,parent_pos,child_pos;
    child_pos = transform.getOrigin();
    parent_pos = parent_transform.getOrigin();
    btVector3 vector_dist = - parent_pos + child_pos;
    btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
    rotation_angle = angleDifference(shape_vector,vector_dist);
//    rotation_angle = qAcos(cost);
    rotation_axis = shape_vector.cross(vector_dist);

    btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
    btVector3 btc_bt = rotatedVector(object_rotation,shape_vector);

    current._center_of_mass_rotation = object_rotation;
    current._center_of_mass_world_position = transform.getOrigin() - btc_bt;
    btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);
    current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s

//    qDebug()<<toString(transform.getRotation(),"quat: ");
    btQuaternion slerp = current._center_of_mass_rotation.slerp(previous._center_of_mass_rotation,delta_t/1000);
    previous._center_of_mass_rotation = btQuaternion(0.3,0.4,0.1,0.7);
    btQuaternion conjugate(-previous._center_of_mass_rotation.x(),-previous._center_of_mass_rotation.y(),-previous._center_of_mass_rotation.z(),previous._center_of_mass_rotation.w());
    btQuaternion product( previous._center_of_mass_rotation * conjugate);
    btQuaternion derivated = slerp;
    btQuaternion distance(current._center_of_mass_rotation.inverse()*previous._center_of_mass_rotation);
    derivated *= 2;
    btQuaternion angular_velocity = derivated * conjugate;
//    qDebug()<<"deriv: "<<btQuat2euler(diff);
//    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void InteractiveObject::updateSimulation(float delta_t){
    t_state_data& current = _simulation_information._current;
    t_state_data& previous =_simulation_information._previous;
    btRigidBody * body = get_body();

//    qDebug()<<toString(body->getCenterOfMassPosition());

    current._center_of_mass_world_position = body->getCenterOfMassPosition();
    current._angular_speed=                  body->getAngularVelocity();
    current._center_of_mass_world_speed =    body->getLinearVelocity();
    current._center_of_mass_rotation =       body->getCenterOfMassTransform().getRotation();

    btVector3 simulation_distance = current._center_of_mass_world_position-previous._center_of_mass_world_position;
    _calculated_simulation_speed = simulation_distance/(delta_t/1000);

//    current._rotation = btQuat2euler(rot);
//    current._rotation_vector_diff = current._rotation -previous._rotation;
    if (current._rotation_vector_diff.length() != 0)
        current._rotation_vector_diff.normalize();
}

void InteractiveObject::updateEnergyStructure(float gravity){

    _energy.part_name = _body_part_name;

    _energy.animation.position.x = _animation_information._current._center_of_mass_world_position.x();
    _energy.animation.position.y = _animation_information._current._center_of_mass_world_position.y();
    _energy.animation.position.z = _animation_information._current._center_of_mass_world_position.z();

    _energy.simulation.position.x = _simulation_information._current._center_of_mass_world_position.x();
    _energy.simulation.position.y = _simulation_information._current._center_of_mass_world_position.y();
    _energy.simulation.position.z = _simulation_information._current._center_of_mass_world_position.z();

    _energy.animation.speed = _animation_information._current._center_of_mass_world_speed.length();
    _energy.simulation.speed = (_simulation_information._current._center_of_mass_world_speed.length());

    _energy.animation.aspeed =      rad2deg(_animation_information._current._angular_speed.length());
    _energy.animation.pt_aspeed.x = rad2deg(_animation_information._current._angular_speed.x());
    _energy.animation.pt_aspeed.y = rad2deg(_animation_information._current._angular_speed.y());
    _energy.animation.pt_aspeed.z = rad2deg(_animation_information._current._angular_speed.z());

    _energy.simulation.aspeed =      rad2deg(_simulation_information._current._angular_speed.length());
    _energy.simulation.pt_aspeed.x = rad2deg(_simulation_information._current._angular_speed.x());
    _energy.simulation.pt_aspeed.y = rad2deg(_simulation_information._current._angular_speed.y());
    _energy.simulation.pt_aspeed.z = rad2deg(_simulation_information._current._angular_speed.z());

    _energy.animation.ke  = translationKineticEnergy( _animation_information._current._center_of_mass_world_speed.length(),_mass);
    _energy.simulation.ke = translationKineticEnergy( _simulation_information._current._center_of_mass_world_speed.length(),_mass);

    _energy.animation.pe  = potentialEnergy(_mass,-gravity,_animation_information._current._center_of_mass_world_position.y());
    _energy.simulation.pe = potentialEnergy(_mass,-gravity,_simulation_information._current._center_of_mass_world_position.y());

    btVector3 aspeed_rad_animation, aspeed_rad_simulation;
    aspeed_rad_animation = btVector3(_animation_information._current._angular_speed.x(),
                                     _animation_information._current._angular_speed.y(),
                                     _animation_information._current._angular_speed.z());
    aspeed_rad_simulation = _simulation_information._current._angular_speed;

    if (_animation_information._current._rotation_vector_diff.length() == 0){
//        qDebug()<<"no a speed animation";
        _energy.animation.ake = 0;
    } else {
        _energy.animation.ake = angularKineticEnergy(aspeed_rad_animation,_animation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }
    if (_animation_information._current._rotation_vector_diff.length() == 0){
//        qDebug()<<"no a speed simulation";
        _energy.simulation.ake = 0;
    } else {
        _energy.simulation.ake = angularKineticEnergy(aspeed_rad_simulation,_simulation_information._current._rotation_vector_diff,_shape.get_shape(),_mass);
    }

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

void InteractiveObject::setSimulationTransformFromAnimation(){
    btVector3 translation;
    btTransform transform;

    transform.setIdentity();
    transform.setRotation( _animation_information._current._center_of_mass_rotation);
    translation = _animation_information._current._center_of_mass_world_position;
    transform.setOrigin(translation);

    _original_transform = transform;
}

void InteractiveObject::setSimulationPosition(btTransform transform, float time){

    insertDataToCurves(_curves_steps,time);// records the data for the curves
    t_state_data& current = _animation_information._current;
//    current._rotation = btQuat2euler(transform.getRotation());
//    if (current._rotation.y() > M_PI-0.01 && current._rotation.z() > M_PI-0.01){
//        current._rotation.setY(0);
//        current._rotation.setZ(0);
//    }
//    qDebug()<<toString(current._rotation,"rotation : ");
//    current._center_of_mass_world_position = transform.getOrigin() - rotatedVector(current._rotation,btVector3(_shape.get_shape().y()/2,0,0));
//    current._center_of_mass_world_position = transform.getOrigin() + rotatedVector(current._rotation,btVector3(0,-_shape.get_shape().y()/2,0));
//    current._center_of_mass_world_position = transform.getOrigin() - _animation.centerToBaseVector();

    setSimulationTransformFromAnimation();
    buildMotion();
    btRigidBody * body = get_body();
    body->setLinearVelocity(_animation_information._current._center_of_mass_world_speed);
//    body->setAngularVelocity(_animation_information._current._angular_speed);
//    body->setAngularVelocity(_animation_information._current._angular_speed);
//    qDebug()<<toString(rad2deg(_animation_information._current._angular_speed),"angular speed : ");

//    _animation_information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
    _simulation_information._current._center_of_mass_world_position = body->getCenterOfMassPosition();
    _simulation_information._previous._center_of_mass_world_position = _animation_information._previous._center_of_mass_world_position;

    _simulation_information._current._center_of_mass_world_speed = body->getLinearVelocity();
    _simulation_information._previous._center_of_mass_world_speed = _animation_information._previous._center_of_mass_world_speed;

//    _simulation_information._current._rotation = _animation_information._current._rotation;
//    _simulation_information._previous._rotation = _animation_information._previous._rotation;

//    _simulation_information._current._angular_speed = body->getAngularVelocity();
//    _simulation_information._previous._angular_speed =  _animation_information._previous._angular_speed;

//    _animation_information._previous._rotation = _animation.rotationVector(0.0f);

    _calculated_simulation_speed = _animation_information._current._center_of_mass_world_speed;
}

void InteractiveObject::setInitialPosition(btTransform transform,btTransform parent_transform){

        t_state_data& current = _animation_information._current;

//        current._center_of_mass_rotation= transform.getRotation();
//        current._rotation = btQuat2euler(transform.getRotation());
//        current._center_of_mass_world_position = transform.getOrigin() - rotatedVector(current._rotation,btVector3(_shape.get_shape().y()/2,0,0));

        btScalar rotation_angle;
        btVector3 rotation_axis,parent_pos,child_pos;
        child_pos = transform.getOrigin();
        parent_pos = parent_transform.getOrigin();
        btVector3 vector_dist = - parent_pos + child_pos;
        btVector3 shape_vector = btVector3(0,_shape.get_shape().y()/2,0);
        rotation_angle = angleDifference(shape_vector,vector_dist);
    //    rotation_angle = qAcos(cost);
        rotation_axis = shape_vector.cross(vector_dist);
        btQuaternion object_rotation(btVector3(rotation_axis),rotation_angle);
        btVector3 btc_bt = rotatedVector(object_rotation,shape_vector);

        current._center_of_mass_rotation = object_rotation;
        current._center_of_mass_world_position = transform.getOrigin() - btc_bt;
//        btVector3 animation_distance(current._center_of_mass_world_position - previous._center_of_mass_world_position);
//        current._center_of_mass_world_speed = animation_distance/(delta_t/1000); // the diff value is in ms so a conversion is needed to be in m/s




        setSimulationTransformFromAnimation();
        buildMotion();
        btRigidBody* body = get_body();
        body->setLinearVelocity(_animation_information._current._center_of_mass_world_speed);

//        _animation_information._previous._center_of_mass_world_position = _animation.extremityTranslationVector(0.0f);
        _simulation_information._previous._center_of_mass_world_position = body->getCenterOfMassPosition();
        _simulation_information._previous._center_of_mass_world_speed = body->getLinearVelocity();

//        _animation_information._previous._rotation = _animation.rotationVector(0.0f);

        _calculated_simulation_speed = _animation_information._current._center_of_mass_world_speed;
//        qDebug()<<"initial position"<<toString(body->getCenterOfMassTransform().getOrigin());
//        qDebug()<<"initial rotation"<<toString(body->getCenterOfMassTransform().getRotation());
//        qDebug()<<"initial velocity"<<toString(body->getLinearVelocity());
//        qDebug()<<"initial angvelo"<<toString(body->getAngularVelocity());
}
