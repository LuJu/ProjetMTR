#include "testscene.h"

TestScene::~TestScene(){
}

TestScene::TestScene(QGLContext *context):
    Viewer(context){

}
TestScene::TestScene(QWidget *parent):
    Viewer(parent){

}
TestScene::TestScene():
    Viewer(){

}

void TestScene::draw(){
    Viewer::draw();
//    _simulation->loop();
    bool started;
    _simulation->get_lock()->lockForRead();
    _display = _simulation->get_display_list();
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

void TestScene::displayAnimation(){
//    qDebug()<<"pos"<<_ui->get_camera().get_position().x()<<" "
//                   <<_ui->get_camera().get_position().y()<<" "
//                   <<_ui->get_camera().get_position().z()<<" ";

    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];
    float elapsed = _simulation->get_elapsed_milliseconds();

    btTransform transform;
    btQuaternion quat;
    InteractiveObject * obj;
    for (int i = 0; i < _display.size(); ++i) {
        obj = _display.at(i);
        if (obj->get_animated()){
            transform.setIdentity();
            btVector3 rotation_degrees = obj->get_animation()._current_state._rotation;
            quat.setEuler(deg2rad(rotation_degrees.y()),
                          deg2rad(rotation_degrees.x()),
                          deg2rad(rotation_degrees.z()));
            btVector3 translation(obj->get_animation()._current_state._center_of_mass_world_position);
//            toString("position",translation);
//            qDebug()<<"translation :"<<translation.x()<<" "<<translation.y()<<" "<<translation.z()<<" ";
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


void TestScene::displaySimulation(){
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
}

void TestScene::displayObject(InteractiveObject * obj, QMatrix4x4& P, QMatrix4x4& V, QMatrix4x4& M, QMatrix4x4 pvm){
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

void TestScene::display3DObjects(){
    bindProgram();
    glViewport(0,0,width(),height());
    if (GlobalConfig::is_enabled("display_simulation")) displaySimulation();
    if (GlobalConfig::is_enabled("display_animation")) displayAnimation();
    releaseProgram();
}

void TestScene::init(){
    Viewer::init();

//    OBJLoader loader;
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(3);
//    loader.parseOBJ(":/models/cube.obj");
    _cube_mesh = QSharedPointer<Mesh>(new Mesh);
    MeshUtils::addFlatSurface(_cube_mesh.data());
}

void TestScene::keyPressEvent(QKeyEvent *keyEvent)
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

void TestScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    Viewer::keyReleaseEvent(keyEvent);
}

void TestScene::closeEvent(QCloseEvent * event){
    Viewer::closeEvent(event);
    _simulation->get_lock()->lockForRead();
    _simulation->stop();
    _simulation->get_lock()->unlock();
    QApplication::closeAllWindows();
}

