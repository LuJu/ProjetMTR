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
    BezierPath path[10];
    path[0]._bezier[0] = Point3df(0,10,0);
    path[0]._bezier[1] = Point3df(10,10,0);
    path[0]._bezier[2] = Point3df(40,200,0);
    path[0]._bezier[3] = Point3df(50,200,0);

    path[1]._bezier[0] = Point3df(50,200,0);
    path[1]._bezier[1] = Point3df(60,200,0);
    path[1]._bezier[2] = Point3df(90,-20,0);
    path[1]._bezier[3] = Point3df(100,-20,0);

    path[2]._bezier[0] = Point3df(0,10,0);
    path[2]._bezier[1] = Point3df(0,10,0);
    path[2]._bezier[2] = Point3df(50,38,0);
    path[2]._bezier[3] = Point3df(50,38,0);

    path[3]._bezier[0] = Point3df(50,38,0);
    path[3]._bezier[1] = Point3df(50,38,0);
    path[3]._bezier[2] = Point3df(100,-20,0);
    path[3]._bezier[3] = Point3df(100,-20,0);

//    path[0].compute(path[0]._bezier,10);
//    path[1].compute(path[1]._bezier,10);
//    path[2].compute(path[0]._bezier,2);
//    path[3].compute(path[0]._bezier,1);
//    path[4].compute(path[0]._bezier,0);
    Curve c;
    c.set_color(QColor(255,0,255));
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

    window.setY(50);
    window.setHeight(-100);
    window.setX(-10);
    window.setWidth(150);

    P.ortho(window);
    pvm = P*V*M;
    _program->setUniformValue("P",P);
    _program->setUniformValue("pvm",pvm);

    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,30,30);
    for (int j = 0; j < 4; ++j) {
        path[j].compute(path[j]._bezier,5);
//        MeshUtils::render(path[j]);
    }
//    for (int j = 0; j < _test_curves.size(); ++j) {
        for (int i = 0; i < 2; ++i) {
            for (int k = 0; k < 4; ++k) {
                c.insert(path[i]._bezier[k].x(),path[i]._bezier[k].y());
            }
        }
//        MeshUtils::render(c,1,c.get_color(),2,true);
//    }
    Curve c1;
    for (int i = 0; i < path[0].get_points().size(); ++i) {
        c1.insert(path[0].get_points().at(i).x(),path[0].get_points().at(i).y());
    }
    for (int i = 0; i < path[1].get_points().size(); ++i) {
        c1.insert(path[1].get_points().at(i).x(),path[1].get_points().at(i).y());
    }
    Curve c2 = c1.tangentCurve();
//    MeshUtils::render(c1,1,c.get_color(),2);
//    MeshUtils::render(c2,1,c.get_color(),2);
    Curve c3;
    c3.insert(10,15);
    c3.insert(15,40);
    c3.insert(60,12);
    c3.insert(100,-40);
    c3.toBezier();

    MeshUtils::render(*(c3._bezier),1,c.get_color(),2);
    MeshUtils::render(c3,1,c.get_color(),2);
qDebug()<<c3.size();

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

