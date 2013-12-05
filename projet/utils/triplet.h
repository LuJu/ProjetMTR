#ifndef TRIPLET_H
#define TRIPLET_H

#include <QDebug>
template<class T> class Triplet
{
public:
    inline Triplet();
    inline Triplet(T x, T y, T z);
    inline Triplet(const Triplet<T>& v);

    inline T& operator[](int) const;

    inline T x()const{return _x;}
    inline T y()const{return _y;}
    inline T z()const{return _z;}

    inline void x(const T x){_x=x;}
    inline void y(const T y){_y=y;}
    inline void z(const T z){_z=z;}

protected:
    T _x, _y, _z;
};


template<class T> Triplet<T>::Triplet()
{
}

template<class T> Triplet<T>::Triplet(const Triplet<T>& v) :
    _x(v.x()), _y(v.y()), _z(v.z())
{
}

template<class T> Triplet<T>::Triplet(T x, T y, T z) : _x(x), _y(y), _z(z)
{
}

template<class T> T& Triplet<T>::operator[](int n) const
{
    return ((T*) this)[n];
}

template<class T> bool operator==(const Triplet<T>& a, const Triplet<T>& b)
{
    return a.x() == b.x() && a.y() == b.y() && a.z() == b.z();
}

template<class T> bool operator!=(const Triplet<T>& a, const Triplet<T>& b)
{
    return a.x() != b.x() || a.y() != b.y() || a.z() != b.z();
}


#endif // TRIPLET_H
