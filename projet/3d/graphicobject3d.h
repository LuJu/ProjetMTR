#ifndef GRAPHICOBJECT3D_H
#define GRAPHICOBJECT3D_H

#include <QVector>
#include <QMatrix4x4>
#include "utils/geometry.h"


#include "mesh.h"
#include "transform.h"

class GraphicObject3D
{
public:
    GraphicObject3D(QString path);
    GraphicObject3D();
    ~GraphicObject3D();

    //! Load the file given in path
    void loadFromAsset(QString path);

    //! Renders the mesh of the object
    void render();
    void resetPosition();

    bool get_visible() const {return _visible;}
    void set_visible(bool visible){_visible = visible;}

    bool is_moving() const {return _moving;}
    void set_moving(bool moving){_moving = moving;}

    Mesh * get_mesh() {return _mesh;}
    void set_mesh(Mesh * mesh){_mesh = mesh;}

    int get_type() const {return _type;}
    void set_type(int type){_type = type;}

    Transform& get_transform() {return _transform;}
    void set_transform(Transform& transform){_transform = transform;}
    QMatrix4x4 get_matrix();

protected:
    void computeTransformations();
    int _type;
    Mesh * _mesh;
    bool _visible;
    bool _moving;
    Transform _transform;
    QMatrix4x4 _matrix;
private:
    void __build();
};

#endif // GRAPHICOBJECT3D_H
