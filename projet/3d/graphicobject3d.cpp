#include "3d/graphicobject3d.h"

GraphicObject3D::GraphicObject3D(QString path)
{

    __build();
    _mesh=new Mesh();
    loadFromAsset(path);
}

GraphicObject3D::GraphicObject3D()
{
    __build();
}

void GraphicObject3D::__build(){
    _visible=true;
    _moving=false;
    _mesh=NULL;
    resetPosition();
}

GraphicObject3D::~GraphicObject3D(){
}

QMatrix4x4 GraphicObject3D::get_matrix(){
    computeTransformations();
    return _matrix;
}

void GraphicObject3D::render(){
    if(_mesh==NULL) qWarning()<<"Mesh not defined";
    else _mesh->render();
}

void GraphicObject3D::computeTransformations(){
    _matrix.setToIdentity();
    _matrix.translate(_transform.get_position().x(),_transform.get_position().y(),_transform.get_position().z());
    _matrix.scale(_transform.get_scale().x(),_transform.get_scale().y(),_transform.get_scale().z());
    _matrix.rotate(_transform.get_rotate().x(),1,0,0);
    _matrix.rotate(_transform.get_rotate().y(),0,1,0);
    _matrix.rotate(_transform.get_rotate().z(),0,0,1);
}

void GraphicObject3D::loadFromAsset(QString path){
    if(_mesh==NULL) _mesh=new Mesh();
    _mesh->loadFromFile(path);
}

void GraphicObject3D::resetPosition(){
    _transform.set_position(Point3df(0.0f,0.0f,0.0f));
}

