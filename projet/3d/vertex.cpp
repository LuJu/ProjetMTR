#include "vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(float x, float y, float z)
{
    _point.x(x);
    _point.y(y);
    _point.z(z);
}

Vertex::Vertex(Point3d<float> point)
{
    _point = point;
}

bool operator==(const Vertex& a, const Vertex& b){
    return (b._normal==a._normal && b._point==a._point);
}

void Vertex::set_color(float r, float g, float b, float a){
    _color[0]=r;
    _color[1]=g;
    _color[2]=b;
    _color[3]=a;
}
