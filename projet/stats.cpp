/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
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
    bool started;
    _simulation->get_lock()->lockForRead();

    if (_simulation->get_display_list().size() > 0){
        _display_curves= &_simulation->get_display_list().at(_selected_index)->get_curves();
        _display_curves_steps= &_simulation->get_display_list().at(_selected_index)->get_curves_steps();
    }
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
    QMatrix4x4 M,V,P;
    QRect window;
    float right,top,bottom,value;
    value = bottom = top = right = 0;

    const QList<Curve>& curves= *_display_curves;
    const QList<Curve>& curves_steps= *_display_curves_steps;

    for (int j = 0; j < curves.size(); ++j) {
        if (_curves_displayed.at(j) == true){

            if (curves[j].size()>0){
                value = (curves[j].end()-1).key();
            } else {
                value = 0;
            }
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
    updateProjectionMatrix(P);
    updateViewMatrix(V);
    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,1000);

    for (int j = 0; j < curves.size(); ++j) {
        if(_curves_displayed.at(j) == true) {
            if (!curves[j].isEmpty())
                MeshUtils::render(curves[j],1,QColor(255,255,255),1);
            if (!curves_steps[j].isEmpty())
                MeshUtils::render(curves_steps[j],1,QColor(255,255,255),3,true);
        }
    }
}


void Stats::displayStatsTest(){
    QMatrix4x4 M,V,P;
    QRect window;
    float right,top,bottom,value;
    value = bottom = top = right = 0;

    const QList<Curve>& curves= *_display_curves;
    const QList<Curve>& curves_steps= *_display_curves_steps;

    for (int j = 0; j < curves.size(); ++j) {
        if (_curves_displayed.at(j) == true){

            if (curves[j].size()>0){
                value = (curves[j].end()-1).key();
            } else {
                value = 0;
            }
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

    window.setY(0);
    window.setHeight(30);
    window.setX(0);
    window.setWidth(30);
    P.ortho(window);
    updateProjectionMatrix(P);
    updateViewMatrix(V);
    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,1000);
    Curve c;
    c.set_interpolation(Curve::bezier);
    Curve c2;
    c.insert(0,5);
    c.insert(1,10);
    c.insert(5,8);
    c.insert(15,30);
//    Point3df p(4.5,c.get_value(4.5),0);
//    Point3df p1(6.5,c.get_value(6.5),0);
//    Point3df p2(0.5,c.get_value(0.5),0);
//    c2.insert(p.x(),p.y());
//    c2.insert(p1.x(),p1.y());
//    c2.insert(p2.x(),p2.y());
    for (int i = 0; i < 150; ++i) {
        c2.insert(((float)i)/10.0f,c.get_value(((float)i)/10.0f));
    }
    MeshUtils::render(c,1,QColor(0,0,255),1);
    MeshUtils::render(c2,1,QColor(255,0,0),4,true);
    MeshUtils::render(c.get_bezier(),1,QColor(255,255,255),1);
}
void Stats::display3DObjects(){
    bindProgram();
    glViewport(0,0,width(),height());
    displayStats();
    releaseProgram();
}

void Stats::init(){
    Viewer::init();
    updateMatrices(QMatrix4x4(),QMatrix4x4(),QMatrix4x4());
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(60);
    _selected_index = 0;

    for (int i = 0; i < Part::NUMBER_OF_CURVES; ++i) {
        _curves_displayed.append(false);
    }
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
}

