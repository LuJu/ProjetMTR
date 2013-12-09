#ifndef TRIANGLE_MESH3D_H
#define TRIANGLE_MESH3D_H

#include <iostream>
#include <fstream>
#include <QVector>
#include <QtOpenGL>
#include <QGLWidget>
#include "qgl.h"
#include "core/core.h"
#include "vertex.h"
#include "utils/curve.h"
class Curve;
class Mesh
{
public:
    enum filetype{
        obj
    };
    enum polygontype{
        triangle,
        lines,
        line_strip
    };

    Mesh();
    void render() const;

    void loadFromFile(QString filepath, int filetype = obj);
    GLuint get_texture() const {return _texture;}
    void set_texture(GLuint texture){_texture = texture;}

    polygontype get_type() const {return _type;}
    void set_type(const polygontype& type){_type = type;}

    QVector<unsigned short>& get_polygons() {return _polygons;}
    QVector<Vertex>& get_vertices() {return _vertices;}

    static void render(const Curve& curve);

protected:
    GLuint _texture;
private:
    void fillVertices(const QVector<Point3df>& _temp_vertices,
                      const QVector<Point3df>& _temp_normals,
                      const QVector<Point3dus>& _temp_polygons,
                      const QVector<Point3dus>& _temp_texture_polygons,
                      const QVector<Point3dus>& _temp_normal_polygons);
    void loadFromOBJ(QString filepath);
    void simpleShape();

    QVector<unsigned short> _polygons;
    QVector<Vertex> _vertices;

    polygontype _type;
    Mesh(Mesh& triangle);
};

#endif // TRIANGLE_MESH3D_H
