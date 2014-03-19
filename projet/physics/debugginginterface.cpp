#include "debugginginterface.h"

DebuggingInterface::DebuggingInterface()
{

}

void DebuggingInterface::update(){


    _simulation->get_lock()->lockForRead();
    int index = part_list->currentIndex();
    InteractiveObject * part = _simulation->get_human()->_parts[index];
    InteractiveObject::t_part_info info = part->getEnergyInformation();
    _simulation->get_lock()->unlock();

    animation_position_x->setText(QString::number(info.animation.x));
    animation_position_y->setText(QString::number(info.animation.y));
    animation_position_z->setText(QString::number(info.animation.z));
    animation_speed->setText(QString::number(info.animation.speed));
    animation_aspeed->setText(QString::number(info.animation.aspeed));
    animation_Ke->setText(QString::number(info.animation.ke));
    animation_aKe->setText(QString::number(info.animation.ake));
    animation_Pe->setText(QString::number(info.animation.pe));
    simulation_position_x->setText(QString::number(info.simulation.x));
    simulation_position_y->setText(QString::number(info.simulation.y));
    simulation_position_z->setText(QString::number(info.simulation.z));
    simulation_speed->setText(QString::number(info.simulation.speed));
    simulation_aspeed->setText(QString::number(info.simulation.aspeed));
    simulation_Ke->setText(QString::number(info.simulation.ke));
    simulation_aKe->setText(QString::number(info.simulation.ake));
    simulation_Pe->setText(QString::number(info.simulation.pe));
    mass->setText(QString::number(part->get_mass()));
    length->setText(QString::number(part->get_shape().y()));
    mean_error->setText(QString::number(info.mean_error));
    mean_error_2->setText(QString::number(info.mean_error_2));
    diff_aKe->setText(QString::number(info.ake_diff));
    diff_Ke->setText(QString::number(info.ke_diff));
    diff_Pe->setText(QString::number(info.pe_diff));

}
