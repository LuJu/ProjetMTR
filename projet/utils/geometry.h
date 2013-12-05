#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "extendedtriplet.h"

template<class T> class Vector3d : public ExtendedTriplet<T>
{
public:
    inline Vector3d();
    inline Vector3d(T x, T y, T z);
    inline Vector3d(const Vector3d<T>& v);

    inline T length() const;
    inline T lengthSquared() const;
    //! returns the normalized version of the vector
    inline void normalize();

    inline Vector3d& operator*=(T s);
};

template<class T> Vector3d<T>::Vector3d() :
    ExtendedTriplet<T>(0,0,0)
{
}

template<class T> Vector3d<T>::Vector3d(const Vector3d<T>& v) :
    ExtendedTriplet<T>(v)
{
}

template<class T> Vector3d<T>::Vector3d(T x, T y, T z) :
    ExtendedTriplet<T>(x,y,z)
{
}

template<class T> Vector3d<T>& Vector3d<T>::operator*=(T s)
{
    ExtendedTriplet<T>::_x *= s; ExtendedTriplet<T>::_y *= s; ExtendedTriplet<T>::_z *= s;
    return *this;
}

template<class T> Vector3d<T> operator*(T s, const Vector3d<T>& v)
{
    return Vector3d<T>(s * v.x(), s * v.y(), s * v.z());
}

template<class T> Vector3d<T> operator*(const Vector3d<T>& v, T s)
{
    return Vector3d<T>(s * v.x(), s * v.y(), s * v.z());
}

//! scalar product
template<class T> T operator*(const Vector3d<T>& a, const Vector3d<T>& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

//! vector product
template<class T> Vector3d<T> operator^(const Vector3d<T>& a, const Vector3d<T>& b)
{
    return Vector3d<T>(a.y() * b.z() - a.z() * b.y(),
                      a.z() * b.x() - a.x() * b.z(),
                      a.x() * b.y() - a.y() * b.x());
}


template<class T> Vector3d<T> operator/(const Vector3d<T>& v, T s)
{
    T div = 1 / s;
    Vector3d<T> ret(div * v.x(), div * v.y(), div * v.z());
    return ret;
}



template<class T> T Vector3d<T>::length() const
{
    return (T) sqrt(ExtendedTriplet<T>::_x * ExtendedTriplet<T>::_x +
                    ExtendedTriplet<T>::_y * ExtendedTriplet<T>::_y +
                    ExtendedTriplet<T>::_z * ExtendedTriplet<T>::_z);
}

template<class T> T Vector3d<T>::lengthSquared() const
{
    return ExtendedTriplet<T>::_x * ExtendedTriplet<T>::_x +
           ExtendedTriplet<T>::_y * ExtendedTriplet<T>::_y +
           ExtendedTriplet<T>::_z * ExtendedTriplet<T>::_z;
}

template<class T> void Vector3d<T>::normalize()
{
    T s = 1 / (T) sqrt(ExtendedTriplet<T>::_x * ExtendedTriplet<T>::_x + ExtendedTriplet<T>::_y * ExtendedTriplet<T>::_y + ExtendedTriplet<T>::_z * ExtendedTriplet<T>::_z);
    ExtendedTriplet<T>::_x *= s;
    ExtendedTriplet<T>::_y *= s;
    ExtendedTriplet<T>::_z *= s;
}

template<class T> class Point3d : public ExtendedTriplet<T>
{
public:
    inline Point3d();
    inline Point3d(const Point3d& p);
    inline Point3d(T x, T y, T z);

    inline Point3d& operator*=(T s);

    inline T distanceTo(const Point3d& p) const;
    inline T distanceToSquared(const Point3d& p) const;
    inline T distanceFromOrigin() const;
    inline T distanceFromOriginSquared() const;

    //! Returns the normal vector of three points (not implemented yet)
    static Vector3d<T> normalVector(const Point3d<T>& a, const Point3d<T>& b, const Point3d<T>& c);
};



template<class T> Point3d<T>::Point3d():
    ExtendedTriplet<T>(0,0,0)
{

}

template<class T> Point3d<T>::Point3d(const Point3d<T>& p) :
    ExtendedTriplet<T>(p)
{

}

template<class T> Point3d<T>::Point3d(T x, T y, T z) :
    ExtendedTriplet<T>(x,y,z)
{
}

template<class T> Point3d<T>& Point3d<T>::operator*=(T s)
{
    ExtendedTriplet<T>::_x *= s;
    ExtendedTriplet<T>::_y *= s;
    ExtendedTriplet<T>::_z *= s;
    return *this;
}

//! multiplication with scalar number
template<class T> Point3d<T> operator*(const Point3d<T>& p, T s)
{
    return Point3d<T>(p.x() * s, p.y() * s, p.z() * s);
}

//! redefinition of the above class for commutativity
template<class T> Point3d<T> operator*(T s, const Point3d<T>& p)
{
    return Point3d<T>(p.x() * s, p.y() * s, p.z() * s);
}

template<class T> T Point3d<T>::distanceTo(const Point3d& p) const
{
    return (T) sqrt(distanceToSquared(p));
}

template<class T> T Point3d<T>::distanceToSquared(const Point3d& p) const
{
    return ((p.x() - ExtendedTriplet<T>::_x) * (p.x() - ExtendedTriplet<T>::_x) +
            (p.y() - ExtendedTriplet<T>::_y) * (p.y() - ExtendedTriplet<T>::_y) +
            (p.z() - ExtendedTriplet<T>::_z) * (p.z() - ExtendedTriplet<T>::_z));
}

template<class T> T Point3d<T>::distanceFromOrigin() const
{
    return (T) sqrt(distanceFromOriginSquared());
}

template<class T> T Point3d<T>::distanceFromOriginSquared() const
{
    return ExtendedTriplet<T>::_x * ExtendedTriplet<T>::_x + ExtendedTriplet<T>::_y * ExtendedTriplet<T>::_y + ExtendedTriplet<T>::_z * ExtendedTriplet<T>::_z;
}

template<class T> Vector3d<T> Point3d<T>::normalVector(const Point3d<T>& a, const Point3d<T>& b, const Point3d<T>& c)
{
    Vector3d<T> v;
    Vector3d<T> w;
}

#endif // GEOMETRY_H
