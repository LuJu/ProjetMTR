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
        _timer_update = new QTimer();
        _timer_update->setInterval(50);
        _timer_update->start();
        connect(_timer_update,SIGNAL(timeout()),this,SLOT(update()));
    }
    QTimer * _timer_update;
    DebuggingInterface * _interface;

signals:
    
public slots:
    void update(){
        _interface->update();
    }
    
};

#endif // DEBUGGINGWIDGET_H
