#ifndef HUMANBODY_H
#define HUMANBODY_H

#include <QList>
#include <core/core.h>
#include "interactiveobject.h"
#include "utils/csvparser.h"
#include "bodyinfo.h"
class HumanBody
{
public:
    HumanBody();
    ~HumanBody();

    QList<InteractiveObject * > _parts;
    int _mass;

    void loadObjects(QString path="values.csv");
    void calculateWork();
    QList<InteractiveObject::energy_info> _data_list;
    void saveDataList();
    void setSimulationPosition(float time);
    void updateEnergyInformations(float elapsed,float diff,float gravity);
    btScalar computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation);
};

#endif // HUMANBODY_H
