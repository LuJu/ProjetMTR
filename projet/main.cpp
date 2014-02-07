#include <QApplication>
#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>
#include "testscene.h"
#include "core/core.h"
#include "utils/geometry.h"
#include "utils/typedefinitions.h"
#include "3d/transform.h"
#include <QMatrix4x4>
#include <QVector4D>
#include "utils/curve.h"
#include "ui_Debugging.h"
#include "physics/debuggingwidget.h"

DebuggingWidget * _debugging;
DebuggingInterface * _debugging_ui;



void speedTest(){

}

void customMessageHandler(QtMsgType type, const char *msg)
{
//    if (_debugging_ui == NULL){
//        int a = 4;
//    } else {
//        QString mes = msg;
        _debugging_ui->log(msg);
        Debugger::customMessageHandler(type,msg);

//    }
}

void firstConfiguration(){
    GlobalConfig::defaultValue("zoom",QVariant(1));
    qDebug()<<GlobalConfig::get_int("zoom");

    GlobalConfig::defaultValue("duration",QVariant(10000));
    GlobalConfig::defaultValue("steps_duration",QVariant(100));
    GlobalConfig::defaultValue("coefficient",QVariant(1));
    GlobalConfig::defaultValue("ups",QVariant(30));
    GlobalConfig::defaultValue("body_mass",QVariant(100),true);
    GlobalConfig::defaultValue("input_location",QVariant("values.csv"));

    GlobalConfig::defaultValue("output_fps",QVariant("true"));
    GlobalConfig::defaultValue("debug_output_debug",QVariant("true"));
    GlobalConfig::defaultValue("debug_output_warning",QVariant("true"));
    GlobalConfig::defaultValue("debug_output_critical",QVariant("true"));
    GlobalConfig::defaultValue("display_ui",QVariant("true"));
    GlobalConfig::defaultValue("display_animation",QVariant("true"));
    GlobalConfig::defaultValue("display_simulation",QVariant("true"));
    GlobalConfig::defaultValue("automatic_close",QVariant("true"));
    GlobalConfig::defaultValue("automatic_start",QVariant("true"));
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
            else qWarning()<<"parameter "<<value<<" valid";
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
            GlobalConfig::set_int("duration",value);
            break ;
        case 4:
            GlobalConfig::set_int("coefficient",value);
            break ;
        case 5:
            break ;
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGLFormat b;
    QGLContext context(b);
    TestScene gui;
    TestScene stats(&gui);

    int ret=0;
//    qInstallMsgHandler(Debugger::customMessageHandler); //redirect the messages
    qDebug()<<"LAUNCHING PROGRAM";
    GlobalConfig::loadConfiguration("Windel","ETS");
    int zoom = GlobalConfig::get_int("zoom");
    firstConfiguration();
    parseArguments(argc,argv);
    GlobalConfig::saveConfiguration();
//    GlobalConfig::parseArguments(argc, argv);
    speedTest();

    Simulation * simulation = new Simulation();


    gui._type = 1;
    gui._simulation = simulation;
    stats.setWindowFlags(Qt::Window);
    gui.setWindowTitle("Physics simulation");
    gui.move(0,0);
    gui._main_viewer = true;

    stats._type = 2;
    stats.setWindowFlags( Qt::Window);
    stats._simulation = simulation;
    stats.setWindowTitle("Stats");
    stats.move(gui.width(),0);
    stats._main_viewer = false;

    simulation->init();

    _debugging_ui = NULL;
    _debugging = new DebuggingWidget(&gui);
    _debugging_ui = new DebuggingInterface();
    _debugging->setWindowFlags( Qt::SubWindow | Qt::Window);
    _debugging_ui->setupUi(_debugging);
    _debugging->_interface = _debugging_ui;
    _debugging_ui->_simulation=simulation;
    _debugging->move(gui.width(),gui.height());

    qInstallMsgHandler(customMessageHandler);



    _debugging_ui->init();
    _debugging->init();
    gui.show();
    stats.show();
    _debugging->show();

    if (GlobalConfig::is_enabled("automatic_start"))
        simulation->start();
    ret=app.exec();

    delete simulation;
    delete _debugging;
    delete _debugging_ui;
    return ret;
}


