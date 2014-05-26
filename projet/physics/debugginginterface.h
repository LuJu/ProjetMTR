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
#ifndef DEBBUGINGINTERFACE_H
#define DEBBUGINGINTERFACE_H

#include "ui_Debugging.h"
#include "simulation.h"
#include "humanbody.h"

#include "stats.h"

#include <QTimer>

class Stats;
class DebuggingInterface : public Ui::Debugging
{

public:
    DebuggingInterface();

    Simulation * _simulation;
    void init(){
        part_list->clear();
        _simulation->get_lock()->lockForRead();
        HumanBody * _human = _simulation->get_human();
        for (int i = 0; i < _human->_limbs.size(); ++i) {
            part_list->addItem(_human->_limbs[i]->get_body_part());
        }
        _simulation->get_lock()->unlock();
    }
    void log(const char * msg){
//        console->setText(msg);
//        console->append(QString(msg));
    }

    void update();

    Stats * _stats;
};

#endif // DEBBUGINGINTERFACE_H
