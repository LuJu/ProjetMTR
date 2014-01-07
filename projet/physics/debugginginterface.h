#ifndef DEBBUGINGINTERFACE_H
#define DEBBUGINGINTERFACE_H

#include "ui_Debugging.h"
#include "simulation.h"
#include "humanbody.h"

class DebuggingInterface : public Ui::Debugging
{
//    Q_OBJECT
public:
    DebuggingInterface();
    HumanBody * _human;
    void init(){
        part_list->clear();
        for (int i = 0; i < _human->_parts.size(); ++i) {
            part_list->addItem(_human->_parts[i]->get_body_part());
        }
    }
    void update(){
        int index = part_list->currentIndex();
        InteractiveObject * part = _human->_parts[index];
        speed_x->setText(QString::number(part->getEnergyInformation().animation_speed));


    }
};

#endif // DEBBUGINGINTERFACE_H
