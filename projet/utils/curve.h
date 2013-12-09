#ifndef CURVE_H
#define CURVE_H

#include <QMap>
#include <QColor>
class Curve : public QMap<float,float>
{
public:

    enum Interpolation{
        linear
    };

    Curve();
    Curve(QString label);

    //! returns the value of the curve at the time given in parameter
    /*!
        \n Status  1 : not implemented
        \param  time : the time targeted
        \return the value at the time targeted (an interpolation might be needed
    */
    float get_value (float time) const;

    void set_interpolation(Interpolation interpolation);

    //! returns the interpolation of the values depending on the _interpolation property
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
    float interpolate(float time1,float value1,float time2,float value2, float target) const;

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
    float linearInterpolation(float time1,float value1,float time2,float value2, float target) const;


    const QColor& get_color() const {return _color;}
    void set_color(const QColor& color){_color = color;}
    QString _label;
protected:
    Interpolation _interpolation;
    QColor _color;

};

#endif // CURVE_H
