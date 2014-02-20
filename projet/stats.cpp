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
//    _simulation->get_mutex()->unlock();
    frameEnd();
}

void Stats::displayStats(){
    QMatrix4x4 M;
    QMatrix4x4 V;
    QMatrix4x4 P;
    QMatrix4x4 pvm;
    QRect window;
    pvm = P*V*M;
    _program->setUniformValue("shininess",(GLfloat)1.0);

//    const QList<InteractiveObject * >& display = _simulation->get_display_list();
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
    window.setY(_ui->get_zoom()/10+1);
    window.setHeight((-(_ui->get_zoom()/5)+1));
    window.setX(right-width()*4);
    window.setWidth(width()*4);

//    QRect window(0,top ,right,absolute_value(top-(bottom)));
//    QRect window(0,top+1,right+10,top+1-(bottom-1));
//        P.ortho(-100,100,-100,100,-100,100);
    P.ortho(window);
    Mesh::drawGrid(window,QColor(0,0,0,255),1,1000,1000);

    for (int i = 0; i < _display.size(); ++i) {
        InteractiveObject * obj = _display.at(i);
        const QList<Curve>& curves= _display.at(i)->get_curves();
        M = QMatrix4x4();
        V = QMatrix4x4();
        pvm = P*V*M;
        _program->setUniformValue("M",M);
        _program->setUniformValue("pvm",pvm);
        for (int j = 0; j < curves.size(); ++j) {
//            const Curve& c = obj->_animation_from_simulation->get_translation_curves()[j];
            Mesh::render(curves[j],GlobalConfig::get_int("steps_duration")/100,curves[j].get_color(),i+2);
        }
        const QList<Curve>& curves_steps= _display.at(i)->get_curves_steps();
        for (int j = 0; j < curves_steps.size(); ++j) {
            //            const Curve& c = obj->_animation_from_simulation->get_translation_curves()[j];
            Mesh::render(curves_steps[j],GlobalConfig::get_int("steps_duration")/100,curves_steps[j].get_color(),i+6,true);
        }
    }
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
    _ui->activateProgressiveZoom(60);
    _background_activated=false;
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

