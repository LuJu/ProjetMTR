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

void Scene::displayAnimation(){
    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];

    btTransform transform;
    btQuaternion quat;
    InteractiveObject * obj;
    for (int i = 0; i < _display.size(); ++i) {
        obj = _display.at(i);
        if (obj->get_animated()){
            transform.setIdentity();
            btVector3 rotation_radians = obj->get_animation()._information._current._rotation;
            quat.setEuler(rotation_radians.y(),rotation_radians.x(),rotation_radians.z());
            btVector3 translation(obj->get_animation()._information._current._center_of_mass_world_position);
            transform.setRotation(quat);
            transform.setOrigin(translation);
            transform.getOpenGLMatrix(matrix);
            M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                         matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                         matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                         matrix[3], matrix[7], matrix[11],matrix[15]
                    );
            displayObject(obj,P,V,M,pvm);
        }
    }
}


void Scene::displaySimulation(){
    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];

    for (int i = 0; i < _display.size(); ++i) {
        InteractiveObject * obj = _display.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                     matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                     matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                     matrix[3], matrix[7], matrix[11],matrix[15]
                );
        displayObject(obj,P,V,M,pvm);
    }

    for (int i = 0; i < _scenery.size(); ++i) {
        InteractiveObject * obj = _scenery.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                matrix[3], matrix[7], matrix[11],matrix[15]
                );
        displayObject(obj,P,V,M,pvm);
    }
}

void Scene::displayObject(InteractiveObject * obj, QMatrix4x4& P, QMatrix4x4& V, QMatrix4x4& M, QMatrix4x4 pvm){
    btVector3 local_scale =obj->get_shape_struct().get_shape();
    switch (obj->get_shape_struct().get_shape_type()) {
    case Shape::cube:
    case Shape::cylinder:
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        insertMatrices(P,V,M);
        switch (obj->get_shape_struct().get_shape_type()) {
        case Shape::cube:
            _cube_mesh->render();
            break;
        }
        break;
        case Shape::capsule:
        insertMatrices(P,V,M);
        obj->get_mesh()->render();
        break;
    default:
        break;
    }
}

void Scene::displayPoints(){
    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];

    for (int i = 0; i < _display.size(); ++i) {
        InteractiveObject * obj = _display.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[4] ,matrix[8] ,matrix[12],
                     matrix[1] ,matrix[5] ,matrix[9] ,matrix[13],
                     matrix[2] ,matrix[6] ,matrix[10],matrix[14],
                     matrix[3], matrix[7], matrix[11],matrix[15]
                );
        displayObjectPoints(obj,P,V,M,pvm);
    }
}

void Scene::displayObjectPoints(InteractiveObject * obj, QMatrix4x4& P, QMatrix4x4& V, QMatrix4x4& M, QMatrix4x4 pvm){
    btVector3 local_scale =obj->get_shape_struct().get_shape();
    switch (obj->get_shape_struct().get_shape_type()) {
    case Shape::cube:
    case Shape::cylinder:
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        insertMatrices(P,V,M);
        switch (obj->get_shape_struct().get_shape_type()) {
        case Shape::cube:
            _cube_mesh->render();
            break;
        }
        break;
        case Shape::capsule:
        insertMatrices(P,V,M);
        MeshUtils::render(Point3df(0,0,0));
        break;
    default:
        break;
    }
}

void Scene::display3DObjects(){
    bindProgram();
    glViewport(0,0,width(),height());
    if (GlobalConfig::is_enabled("display_simulation")) displaySimulation();
    if (GlobalConfig::is_enabled("display_animation")) displayAnimation();
    displayPoints();
    releaseProgram();
}

void Scene::init(){
    Viewer::init();
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(3);
    _cube_mesh = QSharedPointer<Mesh>(new Mesh);
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

