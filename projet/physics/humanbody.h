#ifndef HUMANBODY_H
#define HUMANBODY_H

#include <QList>
#include <core/globalconfig.h>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/csvparser.h"

#include "interactiveobject.h"
#include "bodyinfo.h"
class HumanBody
{
public:
    HumanBody();
    ~HumanBody();

    QList<InteractiveObject * > _parts;
    QList<btConstraintArray *> _constraints;

    void loadObjects(QString path);
    void calculateWork();
    QList<InteractiveObject::energy_info> _data_list;
    void saveDataList();
    void setSimulationPosition(float time);
    void updateEnergyInformations(float elapsed,float diff,float gravity);
    btScalar computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation);

    int get_mass() const {return _mass;}
    void set_mass(int mass){_mass = mass;}

    float get_total_work() const{
        float work = 0;
        for (int i = 0; i < _parts.size(); ++i) {
            work+=_parts[i]->getEnergyInformation().work;
        }
        return work;
    }

    QList<InteractiveObject * >::iterator findPartByName(const QString& name){
        QList<InteractiveObject * >::iterator i;
        InteractiveObject * value;
        for (i = _parts.begin(); i != _parts.end(); ++i) {
            value = *i;
            if (value->get_body_part() == name) return i;
        }
        return _parts.end();
    }

protected:
    int _mass;

};

#endif // HUMANBODY_H
