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
    if (!_complete){

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
            if (_parts.second != NULL){
                constraint= new btHingeConstraint(
                            _parts.first->get_body(),
                            _parts.second->get_body(),
                            _localeA,
                            _localeB);
            } else
                qWarning()<<"cannot create hinge constraint on one object";
            break;
        default:
            break;
        }
        _constraint = (btTypedConstraint*) constraint;
    }
}
