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
#include <QApplication>
#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>
#include "scene.h"
#include "graphviewer.h"
#include "core/core.h"
#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include "3d/transform.h"
#include <QMatrix4x4>
#include <QVector4D>
#include "utils/curve.h"
#include "ui_Debugging.h"
#include "physics/debuggingwidget.h"


//! Function to do quick tests
void quickTest(){
}

//Redirects the debug messages
void customMessageHandler(QtMsgType type, const char *msg)
{
    Debugger::customMessageHandler(type,msg);
}
void firstConfiguration(){
    GlobalConfig::defaultValue("zoom",QVariant(100));
    GlobalConfig::defaultValue("duration",QVariant(100000));
    GlobalConfig::defaultValue("steps_duration",QVariant(10000));
    GlobalConfig::defaultValue("coefficient",QVariant(1));
    GlobalConfig::defaultValue("ups",QVariant(60));
    GlobalConfig::defaultValue("body_mass",QVariant(70));
    GlobalConfig::defaultValue("input_location",QVariant("68.1_OK.csv"));
    GlobalConfig::defaultValue("constraints_activated",QVariant("true"));
    GlobalConfig::defaultValue("display_animation",QVariant("true"));
    GlobalConfig::defaultValue("display_simulation",QVariant("true"));
    GlobalConfig::defaultValue("automatic_close",QVariant("true"));
    GlobalConfig::defaultValue("automatic_start",QVariant("true"));
    GlobalConfig::defaultValue("root_fixed",QVariant("false"));
    GlobalConfig::defaultValue("use_csv_masses",QVariant("true"));
    GlobalConfig::defaultValue("display_simulation_window",QVariant("true"));
    GlobalConfig::defaultValue("write_file_complete",QVariant("true"));
    GlobalConfig::defaultValue("write_file_full",QVariant("true"));
    GlobalConfig::defaultValue("write_file",QVariant("true"));
    GlobalConfig::defaultValue("write_file_segments",QVariant("true"));
}

QString usage(){
    QString use;
    use  = "USAGE : ";
    use.append("program ");
    use.append("[input file ");
    use.append("[body total mass ");
    use.append("[steps duration ");
    use.append("]]]");
    return use;
}

void parseArguments(int argc, char *argv[]){
    int value=0;
    bool ok=false;
    for (int i = 1; i < argc; ++i) {
        QString param(argv[i]);
        qDebug()<<"param "<<i<<": "<<param;
        if(i != 1){
            value= param.toInt(&ok);
            if (!ok)
                qWarning()<<"parameter "<<i<<" invalid";
        }
        switch (i){
        case 0:
            break ;
        case 1:
            GlobalConfig::set_string("input_location",param);
            break ;
        case 2:
            GlobalConfig::set_int("body_mass",value);
            break ;
        case 3:
            GlobalConfig::set_int("steps_duration",value);
            break ;
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Scene gui;
    GraphViewer * stats;

    DebuggingWidget * _debugging;
    DebuggingInterface * _debugging_ui;

    int ret;

    QGLFormat format;

    qDebug()<<"LAUNCHING PROGRAM FACIAL ANIMATION THROUGH EFFORT";
    qDebug()<<usage();
    GlobalConfig::loadConfiguration("ETS","FacialAnimation");
    firstConfiguration();
    parseArguments(argc,argv);
    GlobalConfig::saveConfiguration();
    quickTest();

    Simulation * simulation = new Simulation();

    if (GlobalConfig::is_enabled("display_simulation_window"))
    {
        gui._simulation = simulation;
        gui.setWindowTitle("Physics simulation");
        gui.move(0,0);

        _debugging_ui = NULL;
        _debugging = new DebuggingWidget(&gui);
        _debugging_ui = new DebuggingInterface();

        _debugging->setWindowFlags( Qt::SubWindow | Qt::Window);
        _debugging_ui->setupUi(_debugging);
        _debugging->_interface = _debugging_ui;
        _debugging_ui->_simulation=simulation;

        stats=new GraphViewer(&gui);
        stats->setWindowFlags( Qt::Window);
        stats->_simulation = simulation;
        stats->setWindowTitle("Stats");
        stats->move(gui.width(),0);
        stats->setGeometry(gui.width(),0,700,300);
    }
#ifdef QT_4_
        qInstallMsgHandler(customMessageHandler);
#else
        qInstallMessageHandler(customMessageHandler);
#endif

    SimulationParameters params;
    params.set_gravity(btVector3(0,-9.8,0));
    params.set_ups(GlobalConfig::get_int("ups"));
    params.set_coefficient(GlobalConfig::get_int("coefficient"));
    params.set_duration(GlobalConfig::get_int("duration"));
    params.set_steps_duration(GlobalConfig::get_int("steps_duration"));
    params.set_delta_duration(GlobalConfig::get_int("delta_duration"));
    params.set_body_mass(GlobalConfig::get_int("body_mass"));
    params.set_input_location(GlobalConfig::get_string("input_location"));
    params.set_automatic_close(GlobalConfig::is_enabled("automatic_close"));
    simulation->init(params);


    if (GlobalConfig::is_enabled("automatic_start")) simulation->start();
    if (GlobalConfig::is_enabled("display_simulation_window"))
    {
        gui.show();
        stats->show();
        _debugging->show();
        _debugging_ui->init();
        _debugging->init();
        _debugging->move(gui.width(),600);
        _debugging_ui->_stats = stats;
        _debugging_ui->_scene = &gui;
    }

    ret=app.exec();
    delete simulation;
    if (GlobalConfig::is_enabled("display_simulation_window")) {
        delete stats;
        delete _debugging;
        delete _debugging_ui;
    }
    return ret;
}


