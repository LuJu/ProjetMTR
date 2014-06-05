/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "scene.h"

Scene::~Scene(){
}

Scene::Scene(QGLContext *context):
    Viewer(context){

}
Scene::Scene(QWidget *parent):
    Viewer(parent){

}
Scene::Scene():
    Viewer(){

}

void Scene::draw(){
    Viewer::draw();
//    _simulation->loop();
    bool started;
    _simulation->get_lock()->lockForRead();
    _display = _simulation->get_display_list();
    _scenery = _simulation->get_scenery();
    started = _simulation->is_initiated();
    _simulation->get_lock()->unlock();
    if(_simulation->is_over() && GlobalConfig::is_enabled("automatic_close")){
        _simulation->get_lock()->unlock();
        close();
    } else if (started) {
        display3DObjects();
    }
    frameEnd();
}

void Scene::displayObjects(){
    QMatrix4x4 M;
    btScalar matrix[16];
    bool display_simulation= GlobalConfig::is_enabled("display_simulation");
    bool display_animation= GlobalConfig::is_enabled("display_animation");

    btTransform transform;
    Part * obj;
    for (int i = 0; i < _display.size(); ++i) {

        if (i == _selected_index)
            _program->setUniformValue("intensity",1.0f);
        else _program->setUniformValue("intensity",0.5f);

        obj = _display.at(i);
        if (obj->get_animated()){
            if (display_animation){
                // displaying animation object
                transform.setIdentity();
                btQuaternion rotation_quat = obj->_animation_information._current._center_of_mass_rotation;
                btVector3 translation(obj->_animation_information._current._center_of_mass_world_position);
                transform.setRotation(rotation_quat);
                transform.setOrigin(translation);
                transform.getOpenGLMatrix(matrix);
                M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                             matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                             matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                             matrix[3], matrix[7], matrix[11],matrix[15]
                        );
                displayObject(obj,M);
            }
            if (display_simulation){
                // displaying simulation object
                obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
                obj->get_body()->getCenterOfMassPosition();
                M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                             matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                             matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                             matrix[3], matrix[7], matrix[11],matrix[15]
                        );
                displayObject(obj,M);
            }
        }
    }

    _program->setUniformValue("intensity",1.0f);
    for (int i = 0; i < _scenery.size(); ++i) {
        Part * obj = _scenery.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                     matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                     matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                     matrix[3], matrix[7], matrix[11],matrix[15]
                );
        displayObject(obj,M);
    }

}

void Scene::displayObject(Part * obj,QMatrix4x4& M){
    btVector3 local_scale =obj->get_shape_struct().get_shape();
    switch (obj->get_shape_struct().get_shape_type()) {
    case Shape::cube:
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        updateModelMatrix(M);
        _cube_mesh->render();
        break;
        case Shape::capsule:
        updateModelMatrix(M);
        obj->get_mesh()->render();
        break;
    default:
        break;
    }
}

void Scene::displayPoints(){
    QMatrix4x4 M;
    btScalar matrix[16];

    for (int i = 0; i < _display.size(); ++i) {
        Part * obj = _display.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                     matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                     matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                     matrix[3], matrix[7], matrix[11],matrix[15]
                );
        displayObjectPoints(obj,M);
    }
}

void Scene::displayObjectPoints(Part * obj,QMatrix4x4& M){
    btVector3 local_scale =obj->get_shape_struct().get_shape();
    switch (obj->get_shape_struct().get_shape_type()) {
    case Shape::cube:
    case Shape::cylinder:
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        updateModelMatrix(M);
        switch (obj->get_shape_struct().get_shape_type()) {
        case Shape::cube:
            _cube_mesh->render();
            break;
        }
        break;
        case Shape::capsule:
        updateModelMatrix(M);
        MeshUtils::render(Point3df(0,0,0));
        break;
    default:
        break;
    }
}

void Scene::display3DObjects(){
    bindProgram();
    glViewport(0,0,width(),height());
    displayObjects();
    releaseProgram();
}

void Scene::init(){
    Viewer::init();
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(3);
    _cube_mesh = QSharedPointer<Mesh>(new Mesh);
    _selected_index = 0;
    MeshUtils::addFlatSurface(_cube_mesh.data());
}

void Scene::keyPressEvent(QKeyEvent *keyEvent)
{
    bool simulation_started = false;
    _simulation->get_lock()->lockForRead();
    simulation_started = _simulation->is_started();
    _simulation->get_lock()->unlock();

    if(keyEvent->key()==Qt::Key_Space && !simulation_started){
        _simulation->get_lock()->lockForRead();
        _simulation->start();
        _simulation->get_lock()->unlock();
    } else {
        Viewer::keyPressEvent(keyEvent);
    }
}

void Scene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    Viewer::keyReleaseEvent(keyEvent);
}

void Scene::closeEvent(QCloseEvent * event){
    Viewer::closeEvent(event);
    _simulation->get_lock()->lockForRead();
    _simulation->stop();
    _simulation->get_lock()->unlock();
    QApplication::closeAllWindows();
}

