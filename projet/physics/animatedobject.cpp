#include "animatedobject.h"

AnimatedObject::AnimatedObject() {
//    _scaling_curves.set_interpolation(Curve::bezier);
    _translation_curves.set_interpolation(Curve::bezier);
    _rotation_curves.set_interpolation(Curve::bezier);
}
