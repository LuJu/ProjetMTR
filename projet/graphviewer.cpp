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
#include "graphviewer.h"

GraphViewer::~GraphViewer(){
}

GraphViewer::GraphViewer(QGLContext *context):
    Viewer(context){

}
GraphViewer::GraphViewer(QWidget *parent):
    Viewer(parent){

}
GraphViewer::GraphViewer():
    Viewer(){
}

void GraphViewer::draw(){
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

void GraphViewer::displayStats(){
    QMatrix4x4 V,P;
    QRectF window;
    float right,value;
    value = right = 0.0f;

    const QList<Curve>& curves=       *_display_curves;
    const QList<Curve>& curves_steps= *_display_curves_steps;

    for (int j = 0; j < curves.size(); ++j) {
        if (_curves_displayed.at(j) == true){
            if (curves[j].size()>0)
                if (value < (curves[j].end()-1).key()) value = (curves[j].end()-1).key();
        }
    }
    right = value;
    float scale_y = (float)(_ui->get_zoom());
    if ( scale_y < 1.0f) scale_y = 1.0f;

    float height_scale_y = (float)height()/100.0f;
    window.setY((scale_y*height_scale_y)/2.0f);
    window.setHeight((-scale_y)*height_scale_y);
    window.setX(right-width()*10.0f);
    window.setWidth(width()*10.0f);
    P.ortho(window);
    updateProjectionMatrix(P);
    updateViewMatrix(V);
    MeshUtils::drawGrid(window,QColor(0,0,0,255),1,1000,1000);

    for (int j = 0; j < curves.size(); ++j) {
        if(_curves_displayed.at(j) == true) {
            if (!curves[j].isEmpty())
                MeshUtils::render(curves[j],1,QColor(255,255,255),1);
            if (!curves_steps[j].isEmpty())
                MeshUtils::render(curves_steps[j],1,QColor(255,0,0),3,true);
        }
    }
}
void GraphViewer::display3DObjects(){
    bindProgram();
    glViewport(0,0,width(),height());
    displayStats();
    releaseProgram();
}

void GraphViewer::init(){
    Viewer::init();
    updateMatrices(QMatrix4x4(),QMatrix4x4(),QMatrix4x4());
    _ui->set_zoom(100);
    _ui->activateProgressiveZoom(4);
    _selected_index = 0;

    for (int i = 0; i < Part::NUMBER_OF_CURVES; ++i) {
        _curves_displayed.append(false);
    }
}

void GraphViewer::keyPressEvent(QKeyEvent *keyEvent)
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

void GraphViewer::keyReleaseEvent(QKeyEvent *keyEvent)
{
    Viewer::keyReleaseEvent(keyEvent);
}

void GraphViewer::closeEvent(QCloseEvent * event){
    Viewer::closeEvent(event);
    _simulation->get_lock()->lockForRead();
    _simulation->stop();
    _simulation->get_lock()->unlock();
    QApplication::closeAllWindows();
}

