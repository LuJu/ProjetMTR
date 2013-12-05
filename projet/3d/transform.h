#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "utils/geometry.h"
#include "utils/typedefinitions.h"
class Transform
{
public:
    Transform();

    const Point3df& get_rotate() const {return _rotate;}
    void set_rotate(Point3df rotate){_rotate = rotate;}
    const Point3df& get_position() const {return _position;}
    void set_position(Point3df position){_position = position;}
    const Point3df& get_scale() const {return _scale;}
    void set_scale(Point3df scale){_scale = scale;}
    void set_scale(float scale){_scale = Point3df(scale,scale,scale);}

private :
    Point3df _rotate;
    Point3df _position;
    Point3df _scale;
};

#endif // TRANSFORM_H
