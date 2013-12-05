#ifndef CURVE_H
#define CURVE_H

#include <QMap>
#include <3d/mesh.h>
class Curve : public QMap<float,float>
{
public:
    enum Interpolation{
        linear
    };

    Curve();

    Interpolation _interpolation;
    float get_value(float time);
    void set_interpolation(Interpolation interpolation);

    void render();

    float interpolate(float time1,float value1,float time2,float value2, float target);
    //! returns the linear interpolation of the values
    /*!
        description
        \n Status  1 : not implemented
        \param  time1 : time for the first value
        \param  value1 : first value
        \param  time2 : time for the second value
        \param  value2 : second value
        \param target : the time targeted for the return value
        \return the value at time target, 0 if the input values are not valid
    */
    float linearInterpolation(float time1,float value1,float time2,float value2, float target);
    float _color[3];


};

#endif // CURVE_H
