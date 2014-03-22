#include "joint.h"

Joint::Joint():
    _constraint(NULL),
    _type(point),
    _limited(false),
    _complete(false)
{
    _parts.first = NULL;
    _parts.second= NULL;
    for (int i = 0; i < 5; ++i) {
        _limit[i]=0;
    }
}

Joint::~Joint(){
    if (_constraint) delete _constraint;
}

void Joint::buildConstraint(){

    btTypedConstraint * constraint;

    if (_constraint != NULL)
        delete _constraint;
    if (!_complete){ // only points

        if (_parts.second != NULL){
            constraint= new btPoint2PointConstraint(
                _parts.first->get_body(),
                _parts.second->get_body(),
                btVector3(0, _parts.first->get_shape().y()/2  +_parts.first->get_shape().x()  ,0),
                btVector3(0,-_parts.second->get_shape().y()/2 -_parts.second->get_shape().x() ,0));
        } else {
            constraint= new btPoint2PointConstraint(
                _parts.first->get_body(),
                btVector3(0,_parts.first->get_shape().y()/2 +_parts.first->get_shape().x(),0));
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
            break;
        case hinge:
            _localeA.setIdentity();_localeA.getBasis().setEulerZYX(0,M_PI_2,0);_localeA.setOrigin(_pivotA);
//            qDebug()<<"A: "<<_pivotA.x()<<" "<<_pivotA.y()<<" "<<_pivotA.z();
            _localeB.setIdentity();_localeB.getBasis().setEulerZYX(0,M_PI_2,0);_localeB.setOrigin(_pivotB);
//            qDebug()<<"B: "<<_pivotB.x()<<" "<<_pivotB.y()<<" "<<_pivotB.z();
            if (_parts.second != NULL){
                constraint= new btHingeConstraint(
                            _parts.first->get_body(),
                            _parts.second->get_body(),
                            _localeA,
                            _localeB);
                ((btHingeConstraint*)constraint)->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
            } else
                qWarning()<<"cannot create hinge constraint on one object";
            break;
        default:
            break;
        }
        _constraint = (btTypedConstraint*) constraint;
    }
}
