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
#include "debugginginterface.h"

DebuggingInterface::DebuggingInterface()
{

}

void DebuggingInterface::update(){


    bool found = false;
    float mass_value;
    btVector3 shape;
    t_part_info info;

    _simulation->get_lock()->lockForRead(); {
        int index = part_list->currentIndex();
        if (index != _stats->_selected_index) _stats->_selected_index = index ;
        if (index != _scene->_selected_index) _scene->_selected_index = index ;

        const QList<Part * >&  limbs =_simulation->get_display_list();
        if (index < limbs.size() && index >= 0){
            found = true;
            const Part * part = limbs[index];
            mass_value = part->get_mass();
            shape = part->get_shape_struct().get_shape();
            info = part->getEnergyInformation();
        }
    } _simulation->get_lock()->unlock();

    if (found){
        animation_position_x->setText(QString::number(info.animation.position.x,'f',4));
        animation_position_y->setText(QString::number(info.animation.position.y,'f',4));
        animation_position_z->setText(QString::number(info.animation.position.z,'f',4));
        animation_speed->setText(QString::number(info.animation.speed,'f',4));
        animation_aspeed->setText(QString::number(info.animation.aspeed,'f',4));
        animation_Ke->setText(QString::number(info.animation.ke,'f',4));
        animation_aKe->setText(QString::number(info.animation.ake,'f',4));
        animation_Pe->setText(QString::number(info.animation.pe,'f',4));
        simulation_position_x->setText(QString::number(info.simulation.position.x,'f',4));
        simulation_position_y->setText(QString::number(info.simulation.position.y,'f',4));
        simulation_position_z->setText(QString::number(info.simulation.position.z,'f',4));
        simulation_speed->setText(QString::number(info.simulation.speed,'f',4));
        simulation_aspeed->setText(QString::number(info.simulation.aspeed,'f',4));
        simulation_Ke->setText(QString::number(info.simulation.ke,'f',4));
        simulation_aKe->setText(QString::number(info.simulation.ake,'f',4));
        simulation_Pe->setText(QString::number(info.simulation.pe,'f',4));
        mass->setText(QString::number(mass_value,'f',3));
        length->setText(QString::number(shape.y(),'f',2));
        diff_aKe->setText(QString::number(info.ake_diff,'f',4));
        diff_Ke->setText(QString::number(info.ke_diff,'f',4));
        diff_Pe->setText(QString::number(info.pe_diff,'f',4));
    }

    fps->setText(QString::number(_scene->get_fps(),'f',0));
    fps_stats->setText(QString::number(_stats->get_fps(),'f',0));

    QList<bool>& cb = _stats->_curves_displayed;
    for (int i = 0; i < _checkboxes.size(); ++i) {
        if (_checkboxes[i]->isChecked() ) cb[i] = true;
        else cb[i] = false;
    }

}
