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
//    const QList<InteractiveObject * >& display = _simulation->get_display_list();
    for (int i = 0; i < _display.size(); ++i) {
        float elapsed = _simulation->get_elapsed_milliseconds();
        InteractiveObject * obj = _display.at(i);
        if (obj->get_animated()){
            btTransform transform;
            transform.setIdentity();
            btQuaternion quat;
            btVector3 rotation_degrees = obj->get_animation().rotationVector(elapsed);
            quat.setEuler(deg2rad(rotation_degrees.x()),
                          deg2rad(rotation_degrees.y()),
                          deg2rad(rotation_degrees.z()));
            btVector3 translation(obj->get_animation().translationVector(elapsed));
            transform.setRotation(quat);
            transform.setOrigin(translation);
            transform.getOpenGLMatrix(matrix);
            M=QMatrix4x4(matrix[0] ,matrix[1] ,matrix[2] ,matrix[3],
                         matrix[4] ,matrix[5] ,matrix[6] ,matrix[7],
                         matrix[8] ,matrix[9] ,matrix[10],matrix[11],
                         matrix[12],matrix[13],matrix[14],matrix[15]
                    );
            M=M.transposed();
//            btVector3 local_scale =btVector3(obj->get_animation().scalingVector(elapsed));
            btVector3 local_scale =obj->get_shape();
            _program->setUniformValue("shininess",0.5f);
            Mesh c;
            switch (obj->get_shape_type()) {
            case InteractiveObject::cube:
                M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
                _program->setUniformValue("M",M);
                pvm = P*V*M;
                _program->setUniformValue("pvm",pvm);
                _cube_mesh.render();
                break;
            case InteractiveObject::cylinder:
                M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
                _program->setUniformValue("M",M);
                pvm = P*V*M;
                _program->setUniformValue("pvm",pvm);
                _cylinder_mesh.render();
                break;
            case InteractiveObject::capsule:
    //            M.scale(1,1,1);
                _program->setUniformValue("M",M);
                pvm = P*V*M;
                _program->setUniformValue("pvm",pvm);
                c.fromCapsuleShape(local_scale.y(),local_scale.x());
                c.render();
            }
        }
    }
    _program->release();
    _program->bind();
}

void TestScene::displaySimulation(){
    QMatrix4x4 V = _ui->get_camera().get_view_matrix();
    QMatrix4x4 P = _ui->get_camera().get_projection_matrix();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];
    M.scale(10,1,10);
    pvm = P*V*M;
    _program->setUniformValue("pvm",pvm);
//    const QList<InteractiveObject * >& display = _simulation->get_display_list();
    for (int i = 0; i < _display.size(); ++i) {
        InteractiveObject * obj = _display.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[1] ,matrix[2] ,matrix[3],
                     matrix[4] ,matrix[5] ,matrix[6] ,matrix[7],
                     matrix[8] ,matrix[9] ,matrix[10],matrix[11],
                     matrix[12],matrix[13],matrix[14],matrix[15]
                );
        M=M.transposed();
        btVector3 local_scale =obj->get_shape();
        Mesh c;
        switch (obj->get_shape_type()) {
        case InteractiveObject::cube:
            M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
            _program->setUniformValue("M",M);
            pvm = P*V*M;
            _program->setUniformValue("pvm",pvm);
            _cube_mesh.render();
            break;
        case InteractiveObject::cylinder:
            M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
            _program->setUniformValue("M",M);
            pvm = P*V*M;
            _program->setUniformValue("pvm",pvm);
            _cylinder_mesh.render();
            break;
        case InteractiveObject::capsule:
//            M.scale(1,1,1);
            _program->setUniformValue("M",M);
            pvm = P*V*M;
            _program->setUniformValue("pvm",pvm);
            c.fromCapsuleShape(local_scale.y(),local_scale.x());
            c.render();
//_cylinder_mesh.render();
            break;
        default:
            break;
        }
    }
}


void TestScene::display3DObjects(){
    _program->bind();
    _program->setUniformValue("shininess",(GLfloat)1.0);
    glViewport(0,0,width(),height());
//    _simulation->_mutex.lock();
        if (GlobalConfig::is_enabled("display_simulation")) displaySimulation();
        _program->setUniformValue("shininess",(GLfloat).5);
        if (GlobalConfig::is_enabled("display_animation")) displayAnimation();
//    _simulation->_mutex.unlock();
    _program->release();
}

void TestScene::init(){
    Viewer::init();
    _ui->activateProgressiveZoom(60);
    _background_activated=false;

//    _cube_mesh.capsule(1,2);
    _cube_mesh.loadFromFile(":/models/cube.obj");
    _cylinder_mesh.loadFromFile(":/models/cylinder.obj");

    _capsule_mesh.fromCapsuleShape(1,1);
//    _cylinder_mesh.loadFromFile(":/models/Bane_3.obj");
//    _cube_mesh.loadFromFile(":assets/models/cube.obj");
//    _cylinder_mesh.loadFromFile(":assets/models/Bane_3.obj");
    loadTexture(":/textures/models/Bane3_Chest_D.png");
    _cylinder_mesh._texture=_textures[0];

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
//    QGLViewer::closeEvent(event);
}

