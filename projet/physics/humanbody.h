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

    void loadObjects(QString path);
    void calculateWork();
    QList<InteractiveObject::energy_info> _data_list;
    void saveDataList();
    void setSimulationPosition(float time);
    void updateEnergyInformations(float elapsed,float diff,float gravity);
    btScalar computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation);

    int get_mass() const {return _mass;}
    void set_mass(int mass){_mass = mass;}

protected:
    int _mass;

};

#endif // HUMANBODY_H
