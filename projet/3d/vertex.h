#ifndef VERTEX_H
#define VERTEX_H

#include "utils/geometry.h"
#include "utils/typedefinitions.h"

class Vertex
{
public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(Point3d<float> point);
    bool operator=(const Vertex &b);
    void set_color(float r, float g, float b, float a);
//    texture
//    color
    Point3df _point;
    Point3df _normal;
    float _color[4];

    bool _display_normal;
    bool _display_textures;
    bool _display_colors;
};
bool operator==(const Vertex& a, const Vertex& b);
#endif // VERTEX_H
