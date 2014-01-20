#ifndef DEBUGTIMER_H
#define DEBUGTIMER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class DebugTimer : public QTimer
{
    Q_OBJECT
public:
    explicit DebugTimer(QObject *parent = 0);
    
signals:
    
public slots:
    virtual void start(){
        qDebug()<<"loll";
        QTimer::start();
    }
    
};

#endif // DEBUGTIMER_H
