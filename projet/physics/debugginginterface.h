#ifndef DEBBUGINGINTERFACE_H
#define DEBBUGINGINTERFACE_H

#include "ui_Debugging.h"
#include "simulation.h"
#include "humanbody.h"

#include <QTimer>

class DebuggingInterface : public Ui::Debugging
{

public:
    DebuggingInterface();

    Simulation * _simulation;
    void init(){
        part_list->clear();
        _simulation->get_lock()->lockForRead();
        HumanBody * _human = _simulation->get_human();
        for (int i = 0; i < _human->_parts.size(); ++i) {
            part_list->addItem(_human->_parts[i]->get_body_part());
        }
        _simulation->get_lock()->unlock();
    }
    void log(const char * msg){
//        console->setText(msg);
//        console->append(QString(msg));
    }

    void update();
};

#endif // DEBBUGINGINTERFACE_H
