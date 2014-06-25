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
#include "humanbody.h"

HumanBody::HumanBody():
    _total_mass(0)
{
}

HumanBody::~HumanBody(){
    for (int i = 0; i < _limbs.size(); ++i) {
        delete _limbs[i];
    }
    for (int i = 0; i < _joints.size(); ++i) {
        delete _joints[i];
    }
}

void HumanBody::loadObjects(QString path){
    QString filename=path;
    CSVParser list;
    list.parseFile("../assets/CSV/input/"+filename,";");
    QStringList temp;
    int i=0;
    Joint * object;
    QList<Joint*>::iterator joint_parent;
    QList<Part *>::iterator part_child;
    QList<Part *>::iterator part_parent;
    bool ignore = false;
    float mass;
    qDebug()<<"parsing file "<<path;

    QString body_name = path;
    if(body_name.endsWith(".csv",Qt::CaseInsensitive)) body_name.remove(".csv");
    BodyInfo::genFixedInfo("fixed.csv",body_name);

    while (list[i].at(0)!="parts_end"){
        for (i; list[i].at(0)!="end" && i<list.size() ; ++i) {
            temp= list[i];
            if (temp.size() > 0 ){
                if (temp.at(0) == "object") {
                    if (temp.at(2) == "ignore") ignore = true;
                    else ignore = false;
                    if (!ignore){
                        object = new Joint();
                        QString parent = temp.at(3);
                        QString name = temp.at(1);
                        object->_part_name = name;
                        object->_parent_part_name = parent;

                        joint_parent = findJointByPartName(parent);
                        part_child = findPartByName(name);
                        part_parent = findPartByName(parent);

                        if (GlobalConfig::is_enabled("use_csv_masses")){
                            if (BodyInfo::isFixed(name)) mass = 0.0f;
                            else mass = BodyInfo::subSegmentMass(name,_total_mass);
                        }
                        else {
                            if (temp.size() >= 5 && temp.at(4).contains("zero",Qt::CaseInsensitive))
                                mass = 0.0f;
                            else mass = 1.0f;
                            if (temp.size() >= 8){
                                    object->_part_com_proportion = temp.at(5).toFloat();
                                    mass=temp.at(6).toFloat();
                            }
                        }
//                                if (temp.at(7) == "cone") object->_type=Constraint::cone;
//                                else if (temp.at(7) == "hinge") object->_type=Constraint::hinge;
//                                else object->_type=Constraint::point;
                        object->_part_mass=mass;
                    }
                } else if (!ignore) {
                    if (temp.at(0)=="scaling") {
//                        QStringList values = list[i+1+k] ;
                        object->get_animation().get_scaling_curves()[0].insert(0.0f,0.05f);
//                                if (k == 1){
//                                    object->_animation.get_scaling_curves()[0].insert(0.0f,values[j].toFloat());}

                    } else if (temp.at(0)=="translation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                object->get_animation().get_translation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
//                                object->get_animation().get_translation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat()/30);
                            }
                        }
                        btVector3 extends;
                        extends = btVector3(object->get_animation().TranslationVector(0));
//                        if (part_parent != _parts.end()) (*part_parent)->get_shape_struct().set_shape(btVector3(.01,extends.length(),.01));
                    } else if (temp.at(0)=="rotation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                object->get_animation().get_rotation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                            }
                        }
                    }
                }
            }
        }
        if (!ignore){
            _joints.append(object);
        }
        i++;
    }

    ignore = false;
    for (int i = 0; i < _limbs.size(); ++i) {
        _limbs[i]->buildMesh();
    }
    buildJointTree();
    buildConstraints();
}

void HumanBody::recordStatus(){
    part_info full_data;
    for (int i = 0; i < _limbs.size(); ++i) {
        Part * object = _limbs[i];
        part_info energy = object->getEnergyInformation();
        _data_list.append(energy);

        full_data.time+= energy.time;

        full_data.simulation.ake += energy.simulation.ake;
        full_data.simulation.ke += energy.simulation.ke;
        full_data.simulation.pe += energy.simulation.pe;

        full_data.animation.ake += energy.animation.ake;
        full_data.animation.ke += energy.animation.ke;
        full_data.animation.pe += energy.animation.pe;

        full_data.ake_diff += energy.ake_diff;
        full_data.ke_diff += energy.ke_diff;
        full_data.pe_diff += energy.pe_diff;

    }
    _full_data_list.append(full_data);
}


void HumanBody::saveDataList(){
    for (int j = 0; j < _limbs.size(); ++j) {
        savePartDataList(_limbs[j]->get_body_part());
    }
}

void HumanBody::recordSegmentData(){
    part_info segment_data;
    QList<QString> effective_segments;
    QString segment;
    float time = -1;

    effective_segments.append("Head");
    effective_segments.append("Upper Trunc");
    effective_segments.append("Lower Trunc");

    effective_segments.append("Left Hand");
    effective_segments.append("Left Arm");
    effective_segments.append("Left Fore Arm");
    effective_segments.append("Left Up Leg");
    effective_segments.append("Left Leg");
    effective_segments.append("Left Foot");

    effective_segments.append("Right Hand");
    effective_segments.append("Right Arm");
    effective_segments.append("Right Fore Arm");
    effective_segments.append("Right Up Leg");
    effective_segments.append("Right Leg");
    effective_segments.append("Right Foot");
    for (int i = 0; i < effective_segments.size(); ++i) {
        segment = effective_segments.at(i);
        time = -1;
        for (int j = 0; j < _data_list.size(); ++j) {
            if (i == 1){
                qDebug()<<"head";
            }
            if (BodyInfo::getSegment(_data_list.at(j).part_name) == segment.toLower().replace(" ","")){
                if (time < _data_list.at(j).time){
                    if (time != -1)_segments_data_list.append(segment_data);
                    time = _data_list.at(j).time;
                    segment_data = part_info();
                    segment_data.part_name = segment;
                    segment_data.time = time;
                }
                part_info energy = _data_list.at(j);

                segment_data.time= energy.time;

                segment_data.simulation.ake += energy.simulation.ake;
                segment_data.simulation.ke += energy.simulation.ke;
                segment_data.simulation.pe += energy.simulation.pe;

                segment_data.animation.ake += energy.animation.ake;
                segment_data.animation.ke += energy.animation.ke;
                segment_data.animation.pe += energy.animation.pe;

                segment_data.ake_diff += energy.ake_diff;
                segment_data.ke_diff += energy.ke_diff;
                segment_data.pe_diff += energy.pe_diff;

            }
        }
    }
    _segments_data_list.append(segment_data);
}

void HumanBody::savePartDataList(const QString& part_name) const{
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_"+part_name;
    CSVParser parser;
    parser<<"id"<<"temps"<<
            "x animation"<<"y animation"<<"z animation"<<"x simulation"<<" y simulation"<<"z simulation"<<
            "vitesse animation" <<"EC animation"  <<"ECA animation" <<"EP animation" <<
            "vitesse simulation"<<"EC simulation" <<"ECA simulation"<<"EP simulation"<<
            "EC difference"     <<"ECA difference"<<"EP difference" <<"erreur";
    parser.nextLine();
    for (int i = 0; i < _data_list.size(); ++i) {
        if (_data_list[i].part_name == part_name){
            save=_data_list.at(i);
            parser<<save.part_name<<save.time<<
                    save.animation.position.x     <<save.animation.position.y  <<save.animation.position.z   <<
                    save.simulation.position.x    <<save.simulation.position.y <<save.simulation.position.z  <<
                    save.animation.speed <<save.animation.ke <<save.animation.ake <<save.animation.pe<<
                    save.simulation.speed<<save.simulation.ke<<save.simulation.ake<<save.animation.pe<<
                    save.ke_diff         <<save.ake_diff     <<save.pe_diff;
            parser.nextLine();
        }
    }
    parser.saveInFile(name);



}


void HumanBody::saveSegmentsDataList(){

    QList<QString> effective_segments;
    effective_segments.append("Head");
    effective_segments.append("Upper Trunc");
    effective_segments.append("Lower Trunc");

    effective_segments.append("Left Hand");
    effective_segments.append("Left Arm");
    effective_segments.append("Left Fore Arm");
    effective_segments.append("Left Up Leg");
    effective_segments.append("Left Leg");
    effective_segments.append("Left Foot");

    effective_segments.append("Right Hand");
    effective_segments.append("Right Arm");
    effective_segments.append("Right Fore Arm");
    effective_segments.append("Right Up Leg");
    effective_segments.append("Right Leg");
    effective_segments.append("Right Foot");
    for (int i = 0; i < effective_segments.size(); ++i) {
        saveSegmentDataList(effective_segments.at(i));
    }
}

void HumanBody::saveSegmentDataList(const QString segment_name){

    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_"+segment_name;
    CSVParser parser;
    parser<<"id"<<"temps"<<
            "EC animation"  <<"ECA animation" <<"EP animation" <<
            "EC simulation" <<"ECA simulation"<<"EP simulation"<<
            "EC difference"     <<"ECA difference"<<"EP difference" <<"erreur";
    parser.nextLine();
    for (int i = 0; i < _segments_data_list.size(); ++i) {
        if (_segments_data_list[i].part_name.toLower().replace(" ","") == segment_name.toLower().replace(" ","")){
            save=_segments_data_list.at(i);
            parser<<save.part_name<<save.time<<
                    save.animation.ke <<save.animation.ake <<save.animation.pe<<
                    save.simulation.ke<<save.simulation.ake<<save.simulation.pe<<
                    save.ke_diff         <<save.ake_diff     <<save.pe_diff;
            parser.nextLine();
        }
    }
    parser.saveInFile(name);

}




void HumanBody::saveCompleteDataList() const{
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_complete";
    CSVParser parser;

    parser<<"id"<<"temps"<<
            "x animation"<<"y animation"<<"z animation"<<"x simulation"<<" y simulation"<<"z simulation"<<
            "vitesse animation" <<"EC animation"  <<"ECA animation" <<"EP animation" <<
            "vitesse simulation"<<"EC simulation" <<"ECA simulation"<<"EP simulation"<<
            "EC difference"     <<"ECA difference"<<"EP difference" <<"erreur";
    parser.nextLine();
    for (int i = 0; i <  _complete_data_list.size(); ++i) {
        save=_complete_data_list.at(i);
        parser<<save.part_name<<save.time<<
                save.animation.position.x     <<save.animation.position.y  <<save.animation.position.z   <<
                save.simulation.position.x    <<save.simulation.position.y <<save.simulation.position.z  <<
                save.animation.speed <<save.animation.ke <<save.animation.ake <<save.animation.pe<<
                save.simulation.speed<<save.simulation.ke<<save.simulation.ake<<save.animation.pe<<
                save.ke_diff         <<save.ake_diff     <<save.pe_diff;
         parser.nextLine();
    }
    parser.saveInFile(name);
}

void HumanBody::saveFullDataList(float duration, float steps_duration){
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_full";
    CSVParser parser;
    parser<<"id"<<"temps"<<
            "ECT animation (J)" <<"ECA animation (J)" <<"EC totale animation"<<"EP animation (J)" <<
            "ECT simulation (J)"<<"ECA simulation (J)"<<"EC totale simulation"<<"EP simulation (J)"<<
            "ECT difference (J)"<<"ECA difference (J)"<<"EC totale difference"<<"EP difference (J)"<<"duree(ms):"<<duration<<"pas(ms):"<<steps_duration;
    parser.nextLine();
    for (int i = 0; i < _full_data_list.size(); ++i) {
        save=_full_data_list.at(i);
        parser<<i<<save.time<<
        save.animation.ke <<save.animation.ake <<save.animation.ake+save.animation.ke  <<save.animation.pe <<
        save.simulation.ke<<save.simulation.ake<<save.simulation.ake+save.simulation.ke<<save.simulation.pe<<
        save.ke_diff      <<save.ake_diff      <<save.ake_diff+save.ke_diff           <<save.pe_diff;
        parser.nextLine();
    }
    parser.saveInFile(name);

}
void HumanBody::updateInformationJointTree(float elapsed, float diff,float gravity,JointNode* node, btTransform transform){
    if (node == NULL){
        node = _joints_tree.get_root();
        transform.setIdentity();
        btQuaternion quat;
        quat.setEuler(0,-M_PI_2,0);
        transform.setRotation(quat);
        if (node){
            updateInformationJointTree(elapsed,diff,gravity,node,transform);
            for (int i = 0; i < _limbs.size(); ++i){
                _complete_data_list.append(_limbs[i]->getEnergyInformation());
            }
        } else {
            qWarning()<<"No root in part tree";
        }
    } else {
        btTransform object_transform = node->get_data()->get_animation().getWorldTransform(transform,elapsed);
        Joint * data= node->get_data();
        if (data->get_main_part() != NULL )
            data->get_main_part()->updatePartInfo(elapsed,diff,gravity,object_transform,transform );
        for (int i = 0; i < node->get_number_of_children(); ++i) {
            updateInformationJointTree(elapsed,diff,gravity,node->childAt(i),object_transform);
        }
    }
}


void HumanBody::setSimulationPositionJointTree(float elapsed,JointNode* node, btTransform transform){
    if (node == NULL){
        node = _joints_tree.get_root();
        transform.setIdentity();
        btQuaternion quat;
        quat.setEuler(0,-M_PI_2,0);
        transform.setRotation(quat);
        if (node) setSimulationPositionJointTree(elapsed,node,transform);
        else qWarning()<<"No root in part tree";
    } else {
        btTransform object_transform = node->get_data()->get_animation().getWorldTransform(transform,elapsed);
        Joint * data= node->get_data();
        if (elapsed == 0.0f){
            if(data->get_main_part() != NULL)
                data->get_main_part()->initialTransformStateMatch(object_transform,transform);
        }
        else if(data->get_main_part() != NULL){
            data->get_main_part()->updateStepsCurves(elapsed);// records the data for the curves
            data->get_main_part()->transformStateMatch();
        }
        for (int i = 0; i < node->get_number_of_children(); ++i) {
            setSimulationPositionJointTree(elapsed,node->childAt(i),object_transform);
        }
    }
}
void HumanBody::buildJointTree(){
    Joint * temp;
    Part * new_part;
    JointNode * parent;
    int number_of_joints = _joints.size();
    bool inserted[number_of_joints];
    bool complete = false;
    for (int i = 0; i < number_of_joints; ++i) {
        inserted[i] = false;
    }
    while ( complete == false){
        for (int j = 0; j < number_of_joints; ++j) {
            if(inserted[j] == false){
                temp = _joints[j];
                if (!temp->has_parent()){
                    _joints_tree.addNode(temp->_part_name,temp);
                    inserted[j] = true;
                } else {
                    parent = _joints_tree.get_node_by_name(temp->_parent_part_name);
                    if (parent!=NULL){
                        _joints_tree.addNode(temp->_part_name,temp,parent->get_id());
                        new_part = new Part();
                        temp->set_main_part(new_part);
                        new_part->set_mass(temp->_part_mass);
                        new_part->set_body_part(temp->_part_name);
                        new_part->get_shape_struct().set_shape_type(Shape::capsule);
                        new_part->set_animated(true);
                        btVector3 extends = btVector3(temp->get_animation().TranslationVector(0));
                        new_part->get_shape_struct().set_shape(btVector3(.01,extends.length(),.01));
                        new_part->buildMesh();
                        qDebug()<<"part "<<new_part->get_body_part();
                        _limbs.append(new_part);
                        inserted[j] = true;
                    }
                }
            }
        }
        complete = true;
        for (int i = 0; i < number_of_joints; ++i) {
            if (inserted[i] == false)
                complete = false;
        }
    }
}

void HumanBody::buildConstraints(JointNode * current_node){
    Joint * temp;
    if (current_node == NULL){
        current_node = _joints_tree.get_root();
    }
    if (current_node != NULL){
        temp = current_node->get_data();
        QList<QPair<JointNode*,JointNode*> > list;
        int n = current_node->get_number_of_children();
        for (int i = 0; i < n; ++i) {
            list.append(QPair<JointNode*,JointNode*>(current_node,current_node->childAt(i)));
        }
        for (int i = 0; i < n-1; ++i) {
            for (int j = i; j < n-1 ; ++j) {
                list.append(QPair<JointNode*,JointNode*>(current_node->childAt(i),current_node->childAt(j+1)));
            }
        }
        for (int i = 0; i < list.size(); ++i) {
            if (list.at(i).first->get_data()->get_main_part()){
//                Constraint c(list.at(i).first->get_data()->_main_part,list.at(i).second->get_data()->_main_part);
                if (list.at(i).first == current_node)
                    _constraints.append(Constraint(list.at(i).first->get_data()->get_main_part(),list.at(i).second->get_data()->get_main_part(),true));
                else
                    _constraints.append(Constraint(list.at(i).first->get_data()->get_main_part(),list.at(i).second->get_data()->get_main_part()));
            } else { // means that it is root
                if (GlobalConfig::is_enabled("root_fixed"))
                    _constraints.append(Constraint(list.at(i).second->get_data()->get_main_part(),NULL,false));
            }

        }

        for (int i = 0; i < current_node->get_number_of_children(); ++i) {
            buildConstraints(current_node->childAt(i));
        }

    } else {
        qWarning()<<"No root part in tree";
    }
}

QList<Joint*>::iterator HumanBody::findJointByPartName(const QString& name){
    QList<Joint*>::iterator i;
    Joint * value;
    for (i = _joints.begin(); i != _joints.end(); ++i) {
        value = *i;
        if (value->_part_name == name) return i;
    }
    return _joints.end();
}

QList<Part * >::iterator HumanBody::findPartByName(const QString& name){
    QList<Part * >::iterator i;
    Part * value;
    for (i = _limbs.begin(); i != _limbs.end(); ++i) {
        value = *i;
        if (value->get_body_part() == name) return i;
    }
    return _limbs.end();
}
