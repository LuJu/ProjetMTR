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
    started = _simulation->is_started();
    _simulation->get_lock()->unlock();
    if(_simulation->is_over() && GlobalConfig::is_enabled("automatic_close")){
        _simulation->get_lock()->unlock();
        close();
    } else if (started) {
        display3DObjects();
    }
//    _simulation->get_mutex()->unlock();
    frameEnd();
}

void TestScene::displayAnimation(){
    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];
    float elapsed = _simulation->get_elapsed_milliseconds();

    btTransform transform;
    btQuaternion quat;

    for (int i = 0; i < _display.size(); ++i) {
        InteractiveObject * obj = _display.at(i);
        if (obj->get_animated()){
            transform.setIdentity();
            btVector3 rotation_degrees = obj->get_animation().rotationVector(elapsed);
            quat.setEuler(deg2rad(rotation_degrees.x()),
                          deg2rad(rotation_degrees.y()),
                          deg2rad(rotation_degrees.z()));
            btVector3 translation(obj->get_animation().translationVector(elapsed));
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
    Mesh c;
    btVector3 local_scale =obj->get_shape();
    switch (obj->get_shape_type()) {
    case InteractiveObject::cube:
    case InteractiveObject::cylinder:
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        pvm = P*V*M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",pvm);
        switch (obj->get_shape_type()) {
        case InteractiveObject::cube:
            _cube_mesh->render();
            break;
        case InteractiveObject::cylinder:
//            _cylinder_mesh.render();
            break;
        }
        break;
    case InteractiveObject::capsule:
        pvm = P*V*M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",pvm);
        MeshUtils::addCapsuleShape(&c,local_scale.y(),local_scale.x());
        c.render();
        break;
    default:
        break;
    }
}

void TestScene::display3DObjects(){
    _program->bind();
    _program->setUniformValue("shininess",(GLfloat)1.0);
    glViewport(0,0,width(),height());
    if (GlobalConfig::is_enabled("display_simulation")) displaySimulation();
    _program->setUniformValue("shininess",(GLfloat).5);
    if (GlobalConfig::is_enabled("display_animation")) displayAnimation();
    _program->release();
}

void TestScene::init(){
    Viewer::init();
    _ui->activateProgressiveZoom(60);
    OBJLoader loader;
    loader.parseOBJ(":/models/cube.obj");
    _cube_mesh = loader.get_mesh();
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

