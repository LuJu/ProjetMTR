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
    void update();
};

#endif // DEBBUGINGINTERFACE_H
