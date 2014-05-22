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

    QList<InteractiveObject * > _limbs;
    QList<Joint *> _joints;
    QList<Constraint> _constraints;

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

    void updateInformationJointTree(float elapsed, float diff, float gravity, JointNode* node=NULL, btTransform transform=btTransform::getIdentity());
    void setSimulationPositionJointTree(float elapsed, JointNode* node = NULL, btTransform transform =btTransform::getIdentity());

    int get_mass() const {return _mass;}
    void set_mass(int mass){_mass = mass;}

    float get_total_work() const{
        float work = 0;
        for (int i = 0; i < _limbs.size(); ++i) {
            work+=_limbs[i]->getEnergyInformation().ake_diff;
        }
        return work;
    }

    QList<Joint*>::iterator findJointByPartName(const QString& name);


    QList<InteractiveObject * >::iterator findPartByName(const QString& name){
        QList<InteractiveObject * >::iterator i;
        InteractiveObject * value;
        for (i = _limbs.begin(); i != _limbs.end(); ++i) {
            value = *i;
            if (value->get_body_part() == name) return i;
        }
        return _limbs.end();
    }
    void exportSimulationToAnimation();

    WTree<Joint> _joints_tree;

    void buildJointTree();
    void buildConstraints(JointNode * current_node=NULL);
protected:
    int _mass;

};

#endif // HUMANBODY_H
