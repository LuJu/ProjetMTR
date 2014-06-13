/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
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

#include "part.h"
#include "bodyinfo.h"
#include "joint.h"
#include "constraint.h"

//! Contains the information about all parts of the body
typedef WNode<Joint> JointNode;
class HumanBody
{
public:
    HumanBody();
    ~HumanBody();

    QList<Part * > _limbs;
    QList<Joint *> _joints;
    QList<Constraint> _constraints;

    void loadObjects(QString path);
    void recordStatus();
    void recordSegmentData();
    void saveDataList();
    void savePartDataList(const QString& part_name) const;
    void saveFullDataList(float duration, float steps_duration);
    void saveSegmentDataList(const QString segment_name);
    void saveSegmentsDataList();
    void saveCompleteDataList() const;

    void updateInformationJointTree(float elapsed, float diff, float gravity, JointNode* node=NULL, btTransform transform=btTransform::getIdentity());
    void setSimulationPositionJointTree(float elapsed, JointNode* node = NULL, btTransform transform =btTransform::getIdentity());

    int get_mass() const {return _total_mass;}
    void set_mass(int mass){_total_mass = mass;}

protected:
    WTree<Joint> _joints_tree;
    QList<part_info> _data_list;
    QList<part_info> _full_data_list;
    QList<part_info> _complete_data_list;
    QList<part_info> _segments_data_list;
    int _total_mass;

    void exportSimulationToAnimation();
    void buildJointTree();
    void buildConstraints(JointNode * current_node=NULL);

    QList<Joint*>::iterator findJointByPartName(const QString& name);
    QList<Part * >::iterator findPartByName(const QString& name);
};

#endif // HUMANBODY_H
