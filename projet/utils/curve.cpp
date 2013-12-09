#include "curve.h"

Curve::Curve(QString label) :
    _interpolation(linear),
    _label(label) {
}

Curve::Curve() :
    _interpolation(linear) {
}

void Curve::set_interpolation(Interpolation interpolation){
    _interpolation = interpolation;
}


float Curve::get_value(float time) const{
    QMap<float,float>::const_iterator it;
    it = lowerBound(time);
    if (it.key() == time) {
        return it.value();
    } else {
        QMap<float,float>::const_iterator it2 = it;
        it2--;
        interpolate(it2.key(),it2.value(),it.key(),it.value(),time);
    }
}

float Curve::interpolate(float time1,float value1,float time2,float value2, float target) const{
    return linearInterpolation(time1, value1, time2, value2,  target);
}
float Curve::linearInterpolation(float time1,float value1,float time2,float value2, float target) const {
    float interval,interval_target,proportion,diff,value_prop,ret;
    if (target < time1 || target > time2){
        return 0;
    } else {
        interval = time2 - time1;
        interval_target = target - time1;
        proportion = interval_target/interval;
        diff = value2 - value1;
        value_prop = proportion * diff;
        ret = value1 + value_prop;
        return ret;
    }
}




