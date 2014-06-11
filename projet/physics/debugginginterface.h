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

#include "stats.h"
#include "scene.h"

#include <QTimer>

class Stats;
class Scene;

//! Interface for displaying information on the program
class DebuggingInterface : public Ui::Debugging
{

public:
    DebuggingInterface();

    Simulation * _simulation;
    void init(){
        part_list->clear();
        _simulation->get_lock()->lockForRead();
        const QList<Part * >&  limbs = _simulation->get_display_list();
        for (int i = 0; i < limbs.size(); ++i) {
            part_list->addItem(limbs[i]->get_body_part());
        }
        _simulation->get_lock()->unlock();


        _checkboxes.reserve(Part::NUMBER_OF_CURVES);
        for (int i = 0; i < Part::NUMBER_OF_CURVES; ++i) {
            _checkboxes.append(NULL);
        }

        _checkboxes[Part::ANIMATION_AKE] = cb_ecr_anim;
        _checkboxes[Part::ANIMATION_KE] = cb_ec_anim;
        _checkboxes[Part::ANIMATION_PE] = cb_ep_anim;

        _checkboxes[Part::ANIMATION_X] = cb_x_anim;
        _checkboxes[Part::ANIMATION_Y] = cb_y_anim;
        _checkboxes[Part::ANIMATION_Z] = cb_z_anim;

        _checkboxes[Part::SIMULATION_AKE] = cb_ecr_simu;
        _checkboxes[Part::SIMULATION_KE] = cb_ec_simu;
        _checkboxes[Part::SIMULATION_PE] = cb_ep_simu;

        _checkboxes[Part::SIMULATION_X] = cb_x_simu;
        _checkboxes[Part::SIMULATION_Y] = cb_y_simu;
        _checkboxes[Part::SIMULATION_Z] = cb_z_simu;

        _checkboxes[Part::DIFF_AKE] = cb_ecr_diff;
        _checkboxes[Part::DIFF_KE] = cb_ec_diff;
        _checkboxes[Part::DIFF_PE] = cb_ep_diff;


    }
    void log(const char * msg){
//        console->setText(msg);
//        console->append(QString(msg));
    }

    void update();

    Stats * _stats;
    Scene * _scene;
    QList<QCheckBox *> _checkboxes;
};

#endif // DEBBUGINGINTERFACE_H
