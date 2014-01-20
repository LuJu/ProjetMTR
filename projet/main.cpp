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

void speedTest(){
}



DebuggingWidget * _debugging;
DebuggingInterface * _debugging_ui;

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Viewer * gui;
    Viewer * gui_2;

    int ret=0;
//    qInstallMsgHandler(Debugger::customMessageHandler); //redirect the messages
    qDebug()<<"LAUNCHING PROGRAM";
    GlobalConfig::loadConfiguration();
    GlobalConfig::parseArguments(argc, argv);
    speedTest();

    Simulation * simulation = new Simulation();


    gui = new TestScene();
    ((TestScene*) gui)->_type = 1;
    ((TestScene*) gui)->_simulation = simulation;
    gui->setWindowTitle("Physics simulation");
    gui->move(0,0);


    gui_2 = new TestScene();
    ((TestScene*)gui_2)->_type = 2;
    ((TestScene*)gui_2)->_simulation = simulation;
    gui_2->setWindowTitle("Stats");
    gui_2->move(gui->width(),0);

    simulation->standard();
    gui->show();
    gui_2->show();

    _debugging_ui = NULL;
    _debugging = new DebuggingWidget();
    _debugging_ui = new DebuggingInterface();

    _debugging->_interface = _debugging_ui;
    _debugging->init();
    _debugging_ui->setupUi(_debugging);
    _debugging_ui->_human=simulation->get_human();
    _debugging_ui->_simulation=simulation;
    qInstallMsgHandler(customMessageHandler);
    _debugging_ui->init();
    if (GlobalConfig::is_enabled("automatic_start"))
        simulation->startSimulation();
    _debugging->show();
    _debugging->move(0,gui->height());


//    debugging.show();
    ret=app.exec();
    delete gui;
    delete gui_2;
    delete simulation;
    delete _debugging;
    delete _debugging_ui;
    return ret;
}


