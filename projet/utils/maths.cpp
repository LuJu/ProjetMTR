#include "maths.h"
double rad2deg(double angle){
    return (angle/M_PI)*180 ;
}

double deg2rad(double angle){
    return (angle/180)*M_PI;
}


double kinetic_energy(double speed, double mass){
    return (speed*speed*mass)/2;
}

double potential_energy(double mass, double gravitation, double height){
    return (mass*gravitation*height);
}

double absolute_value(double value){
    return (value >= 0)? value : -value;
}
