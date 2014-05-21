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
typedef WNode<Joint> JointNode;
class HumanBody
{
public:
    HumanBody();
    ~HumanBody();

    QList<InteractiveObject * > _parts;
    QList<Joint *> _constraints;
    QList<Constraint> _constraints_list;

    void loadObjects(QString path);
    void loadObjectsOLD(QString path);
    void recordStatus();
    QList<part_info> _data_list;
    QList<part_info> _full_data_list;
    QList<part_info> _complete_data_list;
    void saveDataList();
    void savePartDataList(const QString& part_name) const;
    void saveFullDataList(const SimulationParameters& params);
    void saveCompleteDataList() const;
    void setSimulationPosition(float time);

    void updateInformationJointTree(const JointNode* node, const btTransform& transform, float elapsed, float diff,float gravity);
    void setSimulationPositionJointTree(const JointNode* node, const btTransform& transform, float elapsed);

    void setSimulationJointPosition(float time);
    void updateBodyInformations(float elapsed,float diff,float gravity);

    int get_mass() const {return _mass;}
    void set_mass(int mass){_mass = mass;}

    float get_total_work() const{
        float work = 0;
        for (int i = 0; i < _parts.size(); ++i) {
            work+=_parts[i]->getEnergyInformation().ake_diff;
        }
        return work;
    }

    QList<Joint*>::iterator findJointByPartName(const QString& name);
    QList<Joint*>::iterator findJointByParentPartName(const QString& name);

    QList<InteractiveObject * >::iterator findPartByName(const QString& name){
        QList<InteractiveObject * >::iterator i;
        InteractiveObject * value;
        for (i = _parts.begin(); i != _parts.end(); ++i) {
            value = *i;
            if (value->get_body_part() == name) return i;
        }
        return _parts.end();
    }
    void exportSimulationToAnimation();

    WTree<Joint> _joints_tree;

    void buildJointTree();
    void buildConstraints(JointNode * current_node=NULL);
protected:
    int _mass;

};

#endif // HUMANBODY_H
