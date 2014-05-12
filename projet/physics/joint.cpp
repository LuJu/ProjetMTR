#include "joint.h"

Joint::Joint():
    _constraint(NULL),
    _type(point)
{
    _parts.first = NULL;
    _parts.second= NULL;
}

Joint::Joint(const Joint& other):
    _constraint(NULL),
    _type(other._type)
{
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
}

Joint& Joint::operator=( const Joint& other ) {
    _constraint = NULL;
    _type=other._type;
    _parts.first = other._parts.first;
    _parts.second = other._parts.second;
    return *this;
}

Joint::~Joint(){
    deleteConstraint();
}

void Joint::deleteConstraint(){
    if (_constraint != NULL)_constraint = NULL;
}

void Joint::buildConstraint(){
    btVector3 pivotA, pivotB;
    btTransform localeA, localeB;
    btVector3 shapeA, shapeB  ;
    btRigidBody *bodyA,*bodyB;
    btQuaternion rotationA,rotationB;
    btVector3 rotA,rotB;

    btTypedConstraint * constraint;

    shapeA = _parts.first->get_shape_struct().get_shape();
    bodyA = _parts.first->get_body();

    if (_parts.second != NULL){
        shapeB = _parts.second->get_shape_struct().get_shape();
        bodyB = _parts.second->get_body();
    }

    rotA =_parts.first->_animation.rotationVector(0);
    rotationA.setEuler(0,0,M_PI_2);
    rotationB.setEuler(deg2rad(rotA.y()), deg2rad(rotA.x()), deg2rad(rotA.z()+90));
    pivotA = - btVector3(0,shapeA.y()/2,0);
    pivotB = _parts.first->_animation.extremityTranslationVector(0) - btVector3(0,shapeB.y()/2,0);
    localeA.setIdentity();localeA.setRotation(rotationA);localeA.setOrigin(pivotA);
    localeB.setIdentity();localeB.setRotation(rotationB);localeB.setOrigin(pivotB);

    deleteConstraint();
    switch (_type){
    case cone:

        if (_parts.second != NULL) constraint= new btConeTwistConstraint(*bodyA,*bodyB,localeA,localeB);
        else
            constraint= new btConeTwistConstraint(*bodyA,localeA);

        ((btConeTwistConstraint *)constraint)->setLimit(M_PI,M_PI,0);

        break;
    case hinge:
        if (_parts.second != NULL){
            constraint= new btHingeConstraint(*bodyA,*bodyB,localeA,localeB);
            ((btHingeConstraint*)constraint)->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
        }
        else qWarning()<<"cannot create hinge constraint on one object";
        break;
    default:
        break;
    }
    _constraint = (btTypedConstraint*) constraint;
}
