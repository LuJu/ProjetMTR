#include "stats.h"

Stats::~Stats(){
}

Stats::Stats(QGLContext *context):
    Viewer(context){

}
Stats::Stats(QWidget *parent):
    Viewer(parent){

}
Stats::Stats():
    Viewer(){
}

void Stats::draw(){
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
    frameEnd();
}

void Stats::displayStats(){
    QMatrix4x4 M,V,P,pvm;
    QRect window;
    float right,top,bottom,value;
    value = bottom = top = right = 0;

    for (int i = 0; i < _display.size(); ++i) {
        const QList<Curve>& curves= _display.at(i)->get_curves();
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



    if (GlobalConfig::is_enabled("automatic_stats_progression")){
        window.setY(_ui->get_zoom()/10+1);
        window.setHeight((-(_ui->get_zoom()/5)+1));
        window.setX(right-width()*6);
        window.setWidth(width()*6);

    } else {
        float zoom = _ui->get_zoom();
        float log = 1 / (qLn(zoom) / qLn(10));
        zoom = zoom ;

        qDebug()<<"zoom: "<<log;
        if (log < 0) log = 0;
        log = log * 1000;
        qDebug()<<"log : "<<log;
        int height = log;
        right = _ui->get_camera().get_position().x() * 6;
        top = _ui->get_camera().get_position().y() / 2;
        int topint = top + height/2;
        window.setY(topint);
        window.setHeight(-height);
    //    qDebug()<<"top: "<<top + height/2;
        qDebug()<<"height : "<<- height;
        window.setX(right);
        window.setWidth(width()*6);

    }


    P.ortho(window);
    pvm = P*V*M;
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",pvm);

    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,1000);

    for (int i = 0; i < _display.size(); ++i) {
        const QList<Curve>& curves= _display.at(i)->get_curves();
        for (int j = 0; j < curves.size(); ++j) {
            MeshUtils::render(curves[j],1,curves[j].get_color(),i+1);
//            Mesh::render(curves[j],GlobalConfig::get_int("steps_duration")/10,curves[j].get_color(),i+2);
        }
        const QList<Curve>& curves_steps= _display.at(i)->get_curves_steps();
        for (int j = 0; j < curves_steps.size(); ++j) {
            if (!curves_steps[j].isEmpty()){
                MeshUtils::render(curves_steps[j],1,curves_steps[j].get_color(),i+3,true);
            }

//            MeshUtils::render(curves_steps[j],GlobalConfig::get_int("steps_duration")/10,curves_steps[j].get_color(),i+6,true);
        }
    }
}

void Stats::displayStatsTest(){
    QMatrix4x4 M,V,P,pvm;
    QRect window;
    float right,top,bottom,value;
    value = bottom = top = right = 0;
//    QList<Curve> _test_curves;
    BezierPath path[10];
    Curve curves[6];
    Curve * bcurves[3];
    curves[0] = _display.at(0)->get_animation().get_translation_curves()[0].get_bezier();
    curves[1] = _display.at(0)->get_animation().get_translation_curves()[1].get_bezier();
    curves[2] = _display.at(0)->get_animation().get_translation_curves()[2].get_bezier();
    curves[3] = _display.at(0)->get_animation().get_translation_curves()[0];
    curves[4] = _display.at(0)->get_animation().get_translation_curves()[1];
    curves[5] = _display.at(0)->get_animation().get_translation_curves()[2];
//    _test_curves.append(c);
    for (int j = 0; j < 6; ++j) {
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

    qDebug()<<"window: "<<top<<" "<<bottom<<" "<<right;

    window.setY(10);
    window.setHeight(-20);
//    window.setHeight(-(top+bottom)-20);
    window.setX(-1000);
    window.setWidth(12000);

    P.ortho(window);
    pvm = P*V*M;
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",pvm);

//    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,10);

    for (int i = 0; i < 6; ++i) {
        curves[i].set_color(QColor(255,0,255));
//        MeshUtils::render(curves[i],1,curves[i].get_color(),i+1);right
    }
    MeshUtils::render(curves[0],1 ,QColor(255,0,255),1);
//    qDebug()<<"sizeb:"<<curves[0].size();
//    qDebug()<<"size:"<<curves[3].size();
//    qDebug()<<"value"<<curves[3].value( curves[3].keys()[curves[3].keys().size()-2]);
//    qDebug()<<"value2"<<curves[3].value(curves[3].keys()[curves[3].keys().size()-1]);
    MeshUtils::render(curves[3],1,QColor(0,0,255),1);

}

void Stats::display3DObjects(){
    _program->bind();
    _program->setUniformValue("shininess",(GLfloat)1.0);
    glViewport(0,0,width(),height());
    displayStats();
    _program->release();
}

void Stats::init(){
    Viewer::init();
    _ui->_type = UIState::camera2D;
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(60);
}

void Stats::keyPressEvent(QKeyEvent *keyEvent)
{
    bool simulation_started = false;
    _simulation->get_lock()->lockForRead();
    simulation_started = _simulation->is_started();
    _simulation->get_lock()->unlock();

    if(keyEvent->key()==Qt::Key_Space && !simulation_started){
        _simulation->get_lock()->lockForRead();
        _simulation->start();
        _simulation->get_lock()->unlock();
    } else if (keyEvent->key()==Qt::Key_A) {
        GlobalConfig::switchState("automatic_stats_progression");
    } else {
        Viewer::keyPressEvent(keyEvent);
    }
}

void Stats::keyReleaseEvent(QKeyEvent *keyEvent)
{
    Viewer::keyReleaseEvent(keyEvent);
}

void Stats::closeEvent(QCloseEvent * event){
    Viewer::closeEvent(event);
    _simulation->get_lock()->lockForRead();
    _simulation->stop();
    _simulation->get_lock()->unlock();
    QApplication::closeAllWindows();
//    QGLViewer::closeEvent(event);
}

