#include "joint.h"

Joint::Joint():
    _constraint(NULL),
    _type(point),
    _limited(false),
    _complete(false),
    _pivotA(btScalar(0),btScalar(0),btScalar(0)),
    _pivotB(btScalar(0),btScalar(0),btScalar(0))
{
    _parts.first = NULL;
    _parts.second= NULL;
    for (int i = 0; i < 5; ++i) {
        _limit[i]=0;
    }
}

Joint::Joint(const Joint& other):
    _constraint(NULL),
    _type(other._type),
    _limited(other._limited),
    _complete(other._complete),
    _pivotA(other._pivotA),
    _pivotB(other._pivotB),
    _localeA(other._localeA),
    _localeB(other._localeB)

{
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
    qDebug()<<"copy called";
}

Joint& Joint::operator=( const Joint& other ) {
    _constraint = NULL;
    _type=other._type;
    _limited=other._limited;
    _complete=other._complete;
    _pivotA=other._pivotA;
    _pivotB=other._pivotB;
    _localeA=other._localeA;
    _localeB=other._localeB;
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
    qDebug()<<"copy assignment called";
    return *this;
}



Joint::~Joint(){
    deleteConstraint();
}

void Joint::deleteConstraint(){
    if (_constraint != NULL)
        switch (_type) {
        case point:
            delete (btPoint2PointConstraint*) _constraint;
            break;
        case cone:
            delete (btConeTwistConstraint*) _constraint;
            break;
        case hinge:
            delete (btHingeConstraint*) _constraint;
            break;
        default:
            break;
        }
    _constraint = NULL;
}

void Joint::buildConstraint(){

    btTypedConstraint * constraint;
    btVector3 first_shape = _parts.first->get_shape_struct().get_shape();

    btVector3 second_shape ;
    if (_parts.second != NULL)
        second_shape = _parts.second->get_shape_struct().get_shape();

    deleteConstraint();
    if (!_complete){ // only points

        if (_parts.second != NULL){
            constraint= new btPoint2PointConstraint(
                _parts.first->get_body(),
                _parts.second->get_body(),
                btVector3(0, first_shape.y()/2  + first_shape.x()  ,0),
                btVector3(0,-second_shape.y()/2 - second_shape.x() ,0));
        } else {
            constraint= new btPoint2PointConstraint(
                _parts.first->get_body(),
                btVector3(0,first_shape.y()/2 +first_shape.x(),0));
        }
            _constraint = (btTypedConstraint*) constraint;
    } else {
        switch (_type){
        case point:
            if (_parts.second != NULL){
                constraint= new btPoint2PointConstraint(
                    _parts.first->get_body(),
                    _parts.second->get_body(),
                    _pivotA,
                    _pivotB);
            } else {
                constraint= new btPoint2PointConstraint(
                    _parts.first->get_body(),
                    _pivotA);
            }
            break;
        case cone:
            if (_parts.second != NULL){
                constraint= new btConeTwistConstraint(
                            _parts.first->get_body(),
                            _parts.second->get_body(),
                            _localeA,
                            _localeB);
            } else {
                constraint= new btConeTwistConstraint(
                            _parts.first->get_body(),
                            _localeB);
            }
            ((btConeTwistConstraint *)constraint)->setLimit(M_PI_2,M_PI_2,0);
            break;
        case hinge:
            _pivotA = _parts.second->_animation._current_state._center_of_mass_world_position + _parts.first->_animation.centerToBaseVector(0);
            _pivotB = btVector3(0,-_parts.second->get_shape_struct().get_shape().y()/2,0);
//            _pivotB = _parts.second->_animation._current_state._position;

//            toString(_pivotA,"pA:");
//            toString(_pivotB,"pB:");

            _localeA.setIdentity();_localeA.getBasis().setEulerZYX(0,M_PI_2,0);_localeA.setOrigin(_pivotA);
            _localeB.setIdentity();_localeB.getBasis().setEulerZYX(0,M_PI_2,0);_localeB.setOrigin(_pivotB);
            if (_parts.second != NULL){
                constraint= new btHingeConstraint(
                            _parts.first->get_body(),
                            _parts.second->get_body(),
                            _localeA,
                            _localeB);
                ((btHingeConstraint*)constraint)->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
//                _localeA.setIdentity();_localeA.getBasis().setEulerZYX(0,M_PI_2,0);_localeA.setOrigin(_pivotA);
//                _localeB.setIdentity();_localeB.getBasis().setEulerZYX(0,M_PI_2,0);_localeB.setOrigin(_pivotB);
//                if (_parts.second != NULL){
//                    constraint= new btHingeConstraint(
//                                _parts.first->get_body(),
//                                _parts.second->get_body(),
//                                _localeA,
//                                _localeB);
//                    ((btHingeConstraint*)constraint)->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
            } else
                qWarning()<<"cannot create hinge constraint on one object";
            break;
        default:
            break;
        }
        _constraint = (btTypedConstraint*) constraint;
    }
}

btVector3 Joint::get_world_position_simulation() const{
    btVector3 position;
    btVector3 parent_position = _parts.first->_simulation._current_state._center_of_mass_world_position;
    btVector3 constraint_position ;
    switch (_type){
    case hinge:
    case cone:
        constraint_position = _localeA.getOrigin();
        break;
    case point:
        constraint_position = _pivotA;
        break;
    }
    btVector3 position2;
    btVector3 parent_position2 = _parts.second->_simulation._current_state._center_of_mass_world_position;
    btVector3 constraint_position2 ;
    switch (_type){
    case hinge:
    case cone:
        constraint_position = _localeB.getOrigin();
        break;
    case point:
        constraint_position = _pivotB;
        break;
    }

    position = parent_position + constraint_position;
    position2 = parent_position2 + constraint_position2;
    return position;
}
