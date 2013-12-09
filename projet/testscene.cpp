#include "testscene.h"


TestScene::~TestScene(){

}

void TestScene::draw(){
    Viewer::draw();
//    _simulation.loop();
    display3DObjects();
    frameEnd();
}


void TestScene::displayAnimation(){
    QMatrix4x4 V = _ui->get_view();
    QMatrix4x4 P = _ui->get_projection();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];
    QList<InteractiveObject * >& display = _simulation.get_display_list();
    for (int i = 0; i < display.size(); ++i) {
        float elapsed = _simulation.get_time_simulation();
        InteractiveObject * obj = display.at(i);
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
        btVector3 local_scale =btVector3(obj->get_animation().scalingVector(elapsed));

        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        _program->setUniformValue("M",M);
        pvm = P*V*M;
        _program->setUniformValue("pvm",pvm);
        _program->setUniformValue("shininess",0.5f);
        switch (obj->_shape_type) {
        case InteractiveObject::cube:
            _cube_mesh.render();
            break;
        case InteractiveObject::cylinder:
            _cylinder_mesh.render();
            break;
        default:
            break;
        }
    }
    _program->release();
    glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
    qglColor(Qt::white);
    renderText(10,0,"test",QFont("Arial", 12, QFont::Bold, false));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    _program->bind();
}

void TestScene::displaySimulation(){
    QMatrix4x4 V = _ui->get_view();
    QMatrix4x4 P = _ui->get_projection();
    QMatrix4x4 M;
    QMatrix4x4 pvm;
    btScalar matrix[16];
    M.scale(10,1,10);
    pvm = P*V*M;
    _program->setUniformValue("pvm",pvm);
    _program->setUniformValue("shininess",(GLfloat)1.0);
    QList<InteractiveObject * >& display = _simulation.get_display_list();
    for (int i = 0; i < display.size(); ++i) {
        InteractiveObject * obj = display.at(i);
        obj->get_motion_state()->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
        M=QMatrix4x4(matrix[0] ,matrix[1] ,matrix[2] ,matrix[3],
                     matrix[4] ,matrix[5] ,matrix[6] ,matrix[7],
                     matrix[8] ,matrix[9] ,matrix[10],matrix[11],
                     matrix[12],matrix[13],matrix[14],matrix[15]
                );
        M=M.transposed();
        btVector3 local_scale =obj->get_shape();
        M.scale(local_scale.getX(),local_scale.getY(),local_scale.getZ());
        pvm = P*V*M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",pvm);
        switch (obj->_shape_type) {
        case InteractiveObject::cube:
            _cube_mesh.render();
            break;
        case InteractiveObject::cylinder:
            _cylinder_mesh.render();
            break;
        default:
            break;
        }

        if (GlobalConfig::is_enabled("display_second_viewport")){

            glViewport(0,height()/2,width(),height()/2);
            M = QMatrix4x4();
            QMatrix4x4  P1,V1;
            P1.ortho(-1,2000,-10,10,-100,100);
            V1 = QMatrix4x4();
            pvm = P1*V1*M;
            _program->setUniformValue("M",M);
            _program->setUniformValue("pvm",pvm);
            for (int i = 0; i < 3; ++i) {

                Mesh::render(obj->_animation_from_simulation.get_translation_curves()[i]);
            }
            glViewport(0,0,width(),height()/2);
        }
    }
}

void TestScene::display3DObjects(){
    _program->setUniformValue("shininess",(GLfloat)1.0);

    if (GlobalConfig::is_enabled("display_second_viewport")){
        glViewport(0,0,width(),height()/2);
    } else {
        glViewport(0,0,width(),height());
    }
    if (GlobalConfig::is_enabled("display_simulation")){
        displaySimulation();
    }
    if (GlobalConfig::is_enabled("display_animation")){
        displayAnimation();
    }
}

void TestScene::init(){
    Viewer::init();
    _background_activated=false;

    _cube_mesh.loadFromFile(GlobalConfig::find_asset_path("models/cube.obj"));
    _cylinder_mesh.loadFromFile(GlobalConfig::find_asset_path("models/cylinder.obj"));

    _simulation.standard();
    _simulation.set_autoloop(true);
    _simulation.startSimulation();
}

void TestScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key()==Qt::Key_Space && !_simulation.is_going() && !_simulation.is_over()){
        _simulation.startSimulation();
    } else {
        Viewer::keyPressEvent(keyEvent);
    }
}

void TestScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    Viewer::keyReleaseEvent(keyEvent);
}

