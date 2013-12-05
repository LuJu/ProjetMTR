#include "curve.h"

Curve::Curve()

{
    _interpolation = linear;

}

void Curve::set_interpolation(Interpolation interpolation){

}


float Curve::get_value(float time){
    QMap<float,float>::Iterator it;
    it = lowerBound(time);
    if (it.key() == time) {
        return it.value();
    } else {
        QMap<float,float>::Iterator it2 = it;
        it2--;
        interpolate(it2.key(),it2.value(),it.key(),it.value(),time);
    }
}

float Curve::interpolate(float time1,float value1,float time2,float value2, float target){
    return linearInterpolation(time1, value1, time2, value2,  target);
}
float Curve::linearInterpolation(float time1,float value1,float time2,float value2, float target){
    float tr,tt,p,vr,vt,ret;
    if (target < time1 || target > time2){
        return 0;
    } else {
        tr = time2 - time1;
        tt = target - time1;
        p = tt/tr;
        vr = value2 - value1;
        vt = p * vr;
        ret = value1 + vt;
        return ret;
    }
}

void Curve::render() {
    Mesh mesh;
    mesh.set_type(Mesh::line_strip);
    QMap<float,float>::iterator iterator = begin();
    int i = 0;
    while (iterator != end()){
        Vertex v(iterator.key(),iterator.value(),0);
        v._color[0]=_color[0];
        v._color[1]=_color[1];
        v._color[2]=_color[2];
        mesh.get_vertices().append(v);
        mesh.get_polygons().append(i);
        i++;
        iterator++;
    }
    mesh.render();
}


