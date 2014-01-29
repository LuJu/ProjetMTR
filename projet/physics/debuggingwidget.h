#ifndef DEBUGGINGWIDGET_H
#define DEBUGGINGWIDGET_H

#include <QWidget>
#include "debugginginterface.h"

class DebuggingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DebuggingWidget(QWidget *parent = 0):
        QWidget(parent),
        _timer_update(NULL),
        _interface(NULL){

    }

    ~DebuggingWidget(){
        if (_timer_update) delete _timer_update;
    }

    void init(){
//        _timer_update = new QTimer();
//        _timer_update->setInterval(100);
//        _timer_update->start();
//        connect(_timer_update,SIGNAL(timeout()),this,SLOT(update()));
    }
    QTimer * _timer_update;
    DebuggingInterface * _interface;

signals:
    
public slots:
    void update(){
//        _interface->update();
    }
    
};

#endif // DEBUGGINGWIDGET_H
