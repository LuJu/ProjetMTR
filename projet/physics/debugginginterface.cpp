#include "debugginginterface.h"

DebuggingInterface::DebuggingInterface()
{

}

void DebuggingInterface::update(){


    _simulation->get_lock()->lockForRead();
    int index = part_list->currentIndex();
    if (index != _stats->_selected_index)
        _stats->_selected_index = index ;
    if (index < _simulation->get_human()->_parts.size() && index >= 0){
        InteractiveObject * part = _simulation->get_human()->_parts[index];

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
