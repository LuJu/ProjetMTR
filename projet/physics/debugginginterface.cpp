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


    _simulation->get_lock()->lockForRead();
    int index = part_list->currentIndex();
//    if(GlobalConfig::is_enabled("display_stats"))
    if (index != _stats->_selected_index)
        _stats->_selected_index = index ;
    if (index < _simulation->get_human()->_limbs.size() && index >= 0){
        Part * part = _simulation->get_human()->_limbs[index];

        t_part_info info = part->getEnergyInformation();

        animation_position_x->setText(QString::number(info.animation.position.x));
        animation_position_y->setText(QString::number(info.animation.position.y));
        animation_position_z->setText(QString::number(info.animation.position.z));
        animation_speed->setText(QString::number(info.animation.speed));
        animation_aspeed->setText(QString::number(info.animation.aspeed));
        animation_Ke->setText(QString::number(info.animation.ke));
        animation_aKe->setText(QString::number(info.animation.ake));
        animation_Pe->setText(QString::number(info.animation.pe));
        simulation_position_x->setText(QString::number(info.simulation.position.x));
        simulation_position_y->setText(QString::number(info.simulation.position.y));
        simulation_position_z->setText(QString::number(info.simulation.position.z));
        simulation_speed->setText(QString::number(info.simulation.speed));
        simulation_aspeed->setText(QString::number(info.simulation.aspeed));
        simulation_Ke->setText(QString::number(info.simulation.ke));
        simulation_aKe->setText(QString::number(info.simulation.ake));
        simulation_Pe->setText(QString::number(info.simulation.pe));
        mass->setText(QString::number(part->get_mass()));
        length->setText(QString::number(part->get_shape_struct().get_shape().y()));
        diff_aKe->setText(QString::number(info.ake_diff));
        diff_Ke->setText(QString::number(info.ke_diff));
        diff_Pe->setText(QString::number(info.pe_diff));
    }
    _simulation->get_lock()->unlock();

}
