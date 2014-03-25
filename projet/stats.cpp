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
    window.setY(_ui->get_zoom()/10+1);
    window.setHeight((-(_ui->get_zoom()/5)+1));
    window.setX(right-width()*6);
    window.setWidth(width()*6);

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
            if (!curves_steps[j].isEmpty()) MeshUtils::render(curves_steps[j],1,curves_steps[j].get_color(),i+3,true);
//            Mesh::render(curves_steps[j],GlobalConfig::get_int("steps_duration")/10,curves_steps[j].get_color(),i+6,true);
        }
    }
}

void Stats::displayStatsTest(){
    QMatrix4x4 M,V,P,pvm;
    QRect window;
    float right,top,bottom,value;
    value = bottom = top = right = 0;
    QList<Curve> _test_curves;
    Curve c;
    c.set_color(QColor(255,255,255));
    c.insert(0,10);
    c.insert(5,18);
    c.insert(8,5);
    _test_curves.append(c);
    for (int j = 0; j < _test_curves.size(); ++j) {
        value = (_test_curves[j].end()-1).key();
        if (right < value)
            right = value;
        value = _test_curves[j].get_max();
        if (top < value)
            top = value;
        value = _test_curves[j].get_min();
        if (bottom > value)
            bottom = value;
    }

    window.setY(100);
    window.setHeight(-100);
    window.setX(-10);
    window.setWidth(100);

    P.ortho(window);
    pvm = P*V*M;
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",pvm);


    for (int j = 0; j < _test_curves.size(); ++j) {
        MeshUtils::render(_test_curves[j],1,_test_curves[j].get_color(),2);
    }


//    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,1000);
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

