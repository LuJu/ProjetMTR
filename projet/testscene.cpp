#include "testscene.h"

DebuggingInterface TestScene::_debugging_ui;

TestScene::~TestScene(){

}

void TestScene::draw(){
    Viewer::draw();
//    _simulation.loop();
    display3DObjects();
    _debugging_ui.update();
    if(_simulation.is_over() && GlobalConfig::is_enabled("automatic_close"))
        close();
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
        case InteractiveObject::capsule:
            _capsule_mesh.render();
            break;
        default:
            break;
        }
    }
    _program->release();
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
        case InteractiveObject::capsule:
            _capsule_mesh.render();
            break;
        default:
            break;
        }
    }
}

void TestScene::displayStats(){
    QMatrix4x4 M;
    QMatrix4x4 V;
    QMatrix4x4 P;
    QMatrix4x4 pvm;
    QRect window;
    pvm = P*V*M;
    _program->setUniformValue("shininess",(GLfloat)1.0);

    QList<InteractiveObject * >& display = _simulation.get_display_list();
    float right,top,bottom,value;
    value = bottom = top = right = 0;
    for (int i = 0; i < display.size(); ++i) {
        const QList<Curve>& curves= display.at(i)->_curves;
        for (int j = 0; j < curves.size(); ++j) {
            value = (curves[j].end()-1).key();
            if (right < value)
                right = value;
            value = curves[j].get_max();
            if (top < value)
                top = value;
            value = curves[j].get_min();
            if (bottom > value)
                bottom = value;
        }
    }
    window.setY(_ui->get_zoom()/10+1);
    window.setHeight((-(_ui->get_zoom()/5)+1));
    window.setX(right-width()*4);
    window.setWidth(width()*4);

//    QRect window(0,top ,right,absolute_value(top-(bottom)));
//    QRect window(0,top+1,right+10,top+1-(bottom-1));
//        P.ortho(-100,100,-100,100,-100,100);
    P.ortho(window);
    Mesh::drawGrid(window,QColor(0,0,0,255),1,1000,1000);

    for (int i = 0; i < display.size(); ++i) {
        InteractiveObject * obj = display.at(i);
        const QList<Curve>& curves= display.at(i)->_curves;
        M = QMatrix4x4();
        V = QMatrix4x4();
        pvm = P*V*M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",pvm);
        for (int j = 0; j < curves.size(); ++j) {
//            const Curve& c = obj->_animation_from_simulation.get_translation_curves()[j];
            Mesh::render(curves[j],curves[j].get_color(),i+2);
        }
    }
}

void TestScene::display3DObjects(){
    _program->bind();
    _program->setUniformValue("shininess",(GLfloat)1.0);

    if (GlobalConfig::is_enabled("viewport_fullscreen")) {
        glViewport(0,0,width(),height());
        if (GlobalConfig::is_enabled("display_second_viewport")){
            displayStats();
        } else {
            if (GlobalConfig::is_enabled("display_simulation")){
                displaySimulation();
            }
            if (GlobalConfig::is_enabled("display_animation")){
                displayAnimation();
            }
        }
    }
    _program->release();
//    if (GlobalConfig::is_enabled("display_simulation")){
//        displaySimulation();
//    }
//    if (GlobalConfig::is_enabled("display_animation")){
//        displayAnimation();
//    }
//    if (GlobalConfig::is_enabled("display_second_viewport")){
//        if (GlobalConfig::is_enabled("second_viewport_fullscreen")) {
//            glViewport(0,0,width(),height());
//            displayStats();
//        } else {
//            glViewport(0,height()/2,width(),height()/2);
//            displayStats();
//            glViewport(0,0,width(),height()/2);
//        }
//    }
//    if (GlobalConfig::is_enabled("display_simulation")){
//        displaySimulation();
//    }
//    if (GlobalConfig::is_enabled("display_animation")){
//        displayAnimation();
//    }
}

void TestScene::installDebugger(){
//    debugging.setParent(this);
    _debugging_ui.setupUi(&_debugging);
    _debugging.show();

    qInstallMsgHandler(customMessageHandler);
}

void TestScene::init(){
    Viewer::init();
    _background_activated=false;

    _cube_mesh.loadFromFile(":/models/cube.obj");
    _cylinder_mesh.loadFromFile(":/models/cylinder.obj");
    _capsule_mesh.loadFromFile(":/models/capsule.obj");
//    _cylinder_mesh.loadFromFile(":/models/Bane_3.obj");
//    _cube_mesh.loadFromFile(":assets/models/cube.obj");
//    _cylinder_mesh.loadFromFile(":assets/models/Bane_3.obj");
    loadTexture(":/models/Bane3_Chest_D.png");
    _cylinder_mesh._texture=_textures[0];

    _simulation.standard();
    _simulation.set_autoloop(true);
    if (GlobalConfig::is_enabled("automatic_start"))
        _simulation.startSimulation();
    installDebugger();
    _debugging_ui._human=_simulation.get_human();
    _debugging_ui.init();
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

void TestScene::closeEvent(QCloseEvent * event){
    Viewer::closeEvent(event);
    _debugging.close();
//    QGLViewer::closeEvent(event);
}

