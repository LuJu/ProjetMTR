#ifndef EXTENDEDTRIPLET_H
#define EXTENDEDTRIPLET_H

#include "triplet.h"

template<class T> class ExtendedTriplet : public Triplet<T>
{
public:
    inline ExtendedTriplet();
    inline ExtendedTriplet(T x, T y, T z);
    inline ExtendedTriplet(const ExtendedTriplet<T>& v);

    inline ExtendedTriplet operator+() const;
    inline ExtendedTriplet operator-() const;
    inline ExtendedTriplet& operator+=(const ExtendedTriplet<T>& a);
    inline ExtendedTriplet& operator-=(const ExtendedTriplet<T>& a);
};

template<class T> ExtendedTriplet<T>::ExtendedTriplet()
{
}

template<class T> ExtendedTriplet<T>::ExtendedTriplet(const ExtendedTriplet<T>& v) :
    Triplet<T>(v)
{
}

template<class T> ExtendedTriplet<T>::ExtendedTriplet(T x, T y, T z) :
    Triplet<T>(x,y,z)
{
}

template<class T> ExtendedTriplet<T>& ExtendedTriplet<T>::operator+=(const ExtendedTriplet<T>& a)
{
    Triplet<T>::_x += a.x(); Triplet<T>::_y += a.y(); Triplet<T>::_z += a.z();
    return *this;
}

template<class T> ExtendedTriplet<T>& ExtendedTriplet<T>::operator-=(const ExtendedTriplet<T>& a)
{
    Triplet<T>::_x -= a.x(); Triplet<T>::_y -= a.y(); Triplet<T>::_z -= a.z();
    return *this;
}

template<class T> ExtendedTriplet<T> ExtendedTriplet<T>::operator-() const
{
    return ExtendedTriplet<T>(-Triplet<T>::_x, -Triplet<T>::_y, -Triplet<T>::_z);
}

template<class T> ExtendedTriplet<T> ExtendedTriplet<T>::operator+() const
{
    return *this;
}


template<class T> ExtendedTriplet<T> operator+(const ExtendedTriplet<T>& a, const ExtendedTriplet<T>& b)
{
    return ExtendedTriplet<T>(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

template<class T> ExtendedTriplet<T> operator-(const ExtendedTriplet<T>& a, const ExtendedTriplet<T>& b)
{
    return ExtendedTriplet<T>(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}



#endif // EXTENDEDTRIPLET_H
