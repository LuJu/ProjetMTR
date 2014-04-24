#ifndef HUMANBODY_H
#define HUMANBODY_H

#include <QList>
#include <QDir>
#include <QDateTime>

#include <core/globalconfig.h>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "utils/csvparser.h"

#include "wgraph/wtree.h"

#include "interactiveobject.h"
#include "bodyinfo.h"
#include "joint.h"
#include "simulationparameters.h"

//! Contains the information about all parts of the body
typedef WNode<InteractiveObject> PartNode;
class HumanBody
{
public:
    HumanBody();
    ~HumanBody();

    QList<InteractiveObject * > _parts;
    QList<Joint> _constraints;

    void loadObjects(QString path);
    void recordStatus();
    QList<part_info> _data_list;
    QList<part_info> _full_data_list;
    QList<part_info> _complete_data_list;
    void saveDataList();
    void savePartDataList(const QString& part_name) const;
    void saveFullDataList(const SimulationParameters& params);
    void saveCompleteDataList() const;
    void setSimulationPosition(float time);
    void setSimulationPositionTree(const PartNode* node, const btTransform& transform, float elapsed);
    void updateBodyInformations(float elapsed,float diff,float gravity);
    btScalar computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation);

    int get_mass() const {return _mass;}
    void set_mass(int mass){_mass = mass;}

    float get_total_work() const{
        float work = 0;
        for (int i = 0; i < _parts.size(); ++i) {
            work+=_parts[i]->getEnergyInformation().ake_diff;
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

    QList<InteractiveObject * >::iterator findPartByParentName(const QString& name){
        QList<InteractiveObject * >::iterator i;
        InteractiveObject * value;
        for (i = _parts.begin(); i != _parts.end(); ++i) {
            value = *i;
            if (value->get_parent_body_part() == name) return i;
        }
        return _parts.end();
    }

    void exportSimulationToAnimation();

    WTree<InteractiveObject> _parts_tree;

    //! Builds the part tree (scene graph)
    void buildTree();
    void updateInformationTree(const PartNode* node, const btTransform &transform, float elapsed, float diff, float gravity);
protected:
    int _mass;

};

#endif // HUMANBODY_H
