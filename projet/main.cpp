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

void speedTest(){
    Curve c;
    c.set_interpolation(Curve::closest);
    Curve::const_iterator i = c.begin();
    qDebug()<<i.value();
    c.insert(10,15);
    c.insert(13,17);
    qDebug()<<c.get_min();
    qDebug()<<c.get_max();

    qDebug()<<c.get_value(11.5);
    qDebug()<<c.get_max();

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Viewer * gui;
    int ret=0;
    qInstallMsgHandler(Debugger::customMessageHandler); //redirect the messages
    qDebug()<<"LAUNCHING PROGRAM";
    GlobalConfig::loadConfiguration();
    GlobalConfig::parseArguments(argc, argv);
    speedTest();
    gui = new TestScene();
    gui->setWindowTitle("Physics simulation");
    gui->show();
//    debugging.show();
    ret=app.exec();
    delete gui;
    return ret;
}


