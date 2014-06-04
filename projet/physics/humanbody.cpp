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
    _mass(0)
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

    float order[3],sign[3];
    order[0] = GlobalConfig::get_int("first");
    order[1] = GlobalConfig::get_int("second");
    order[2] = GlobalConfig::get_int("third");
    sign[0] = GlobalConfig::get_int("sfirst");
    sign[1] = GlobalConfig::get_int("ssecond");
    sign[2] = GlobalConfig::get_int("sthird");

    QString filename=path;
    CSVParser list;
//    list.parseFile(":/CSV/input/"+filename,";");
    list.parseFile("../assets/CSV/input/"+filename,";");
    QStringList temp;
    int i=0;
//    InteractiveObject * object = NULL;
    Joint * object;
    Part * new_object = NULL;
    QList<Joint*>::iterator joint_parent;
    QList<Part *>::iterator part_child;
    QList<Part *>::iterator part_parent;
    bool ignore = false;
    float mass;
    qDebug()<<"parsing file "<<path;
    while (list[i].at(0)!="parts_end"){
        for (i; list[i].at(0)!="end" && i<list.size() ; ++i) {
            temp= list[i];
            if (temp.size() > 0 ){
                if (temp.at(0) == "object") {
                    if (temp.at(2) == "ignore") ignore = true;
                    else ignore = false;
                    if (!ignore){
                        object = new Joint();
                        if (temp.size() >= 5 && temp.at(4) == "zeromass") mass = 0.0f;
                        QString parent = temp.at(3);
                        QString name = temp.at(1);
                        object->_part_name = name;
                        object->_parent_part_name = parent;

                        joint_parent = findJointByPartName(parent);
                        part_child = findPartByName(name);
                        part_parent = findPartByName(parent);
                        if (temp.size() >= 8){
                                object->_part_com_proportion = temp.at(5).toFloat();
                                object->_part_mass=temp.at(6).toFloat();
//                                if (temp.at(7) == "cone") object->_type=Constraint::cone;
//                                else if (temp.at(7) == "hinge") object->_type=Constraint::hinge;
//                                else object->_type=Constraint::point;
                        }
                    }
                } else if (!ignore) {
                    if (temp.at(0)=="scaling") {
//                        QStringList values = list[i+1+k] ;
                        object->get_animation().get_scaling_curves()[0].insert(0.0f,0.1f);
//                                if (k == 1){
//                                    object->_animation.get_scaling_curves()[0].insert(0.0f,values[j].toFloat());}

                    } else if (temp.at(0)=="translation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
//                                qDebug()<<"values :"<<values[j];
//                                qDebug()<<"values :"<<values[j+1];
//                                object->_animation.get_translation_curves()[order[k]].insert(values[j].toFloat(),sign[0] * values[j+1].toFloat());
//                                object->get_animation().get_translation_curves()[order[k]].insert(values[j].toFloat(),sign[0] * values[j+1].toFloat());
                                object->get_animation().get_translation_curves()[order[k]].insert(values[j].toFloat(),sign[0] * values[j+1].toFloat()/30);
                            }
                        }
                        btVector3 extends;
                        extends = btVector3(object->get_animation().extremityTranslationVector(0));
                        qDebug()<<extends.length();
//                        if (part_parent != _parts.end()) (*part_parent)->get_shape_struct().set_shape(btVector3(.01,extends.length(),.01));
                    } else if (temp.at(0)=="rotation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                object->get_animation().get_rotation_curves()[order[k]].insert(values[j].toFloat(),sign[k] *values[j+1].toFloat());
                            }
                        }
                    }
                }
            }
        }
        if (!ignore){
            if (new_object ){
//                new_object->get_shape_struct().set_shape_type(Shape::capsule);
//                new_object->set_animated(true);
//                new_object->_simulation.set_shape(&(new_object->_shape));
                mass = BodyInfo::mass(temp.at(1),_mass);
                if (mass==0.0f){
                    qWarning()<<"Object mass null for part: "<<temp.at(1);
                    mass=1;
                }
//                new_object->set_mass(mass);
                object->_part_mass = mass;
                _limbs.append(new_object);
                new_object = NULL;
            }
            _joints.append(object);
        }
        i++;
    }

    ignore = false;
    for (int i = 0; i < _limbs.size(); ++i) {
        _limbs[i]->buildMesh();
    }
//    buildTree();
    buildJointTree();
    buildConstraints();
}

void HumanBody::recordStatus(){
    part_info full_data;
    for (int i = 0; i < _limbs.size(); ++i) {
        Part * object = _limbs[i];
        part_info energy = object->getEnergyInformation();
        _data_list.append(energy);

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

void HumanBody::savePartDataList(const QString& part_name) const{
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_"+part_name;
    CSVParser parser;
    parser<<"id"<<"x animation"<<"y animation"<<"z animation"<<"x simulation"<<" y simulation"<<"z simulation"<<
            "vitesse animation" <<"EC animation"  <<"ECA animation" <<"EP animation" <<
            "vitesse simulation"<<"EC simulation" <<"ECA simulation"<<"EP simulation"<<
            "EC difference"     <<"ECA difference"<<"EP difference" <<"erreur";
    parser.nextLine();
    for (int i = 0; i < _data_list.size(); ++i) {
        if (_data_list[i].part_name == part_name){
            save=_data_list.at(i);
            parser<<save.part_name<<
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


void HumanBody::saveCompleteDataList() const{
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_complete";
    CSVParser parser;

    parser<<"id"<<"x animation"<<"y animation"<<"z animation"<<"x simulation"<<" y simulation"<<"z simulation"<<
            "vitesse animation" <<"EC animation"  <<"ECA animation" <<"EP animation" <<
            "vitesse simulation"<<"EC simulation" <<"ECA simulation"<<"EP simulation"<<
            "EC difference"     <<"ECA difference"<<"EP difference" <<"erreur";
    parser.nextLine();
    for (int i = 0; i <  _complete_data_list.size(); ++i) {
        save=_complete_data_list.at(i);
        parser<<save.part_name<<
                save.animation.position.x     <<save.animation.position.y  <<save.animation.position.z   <<
                save.simulation.position.x    <<save.simulation.position.y <<save.simulation.position.z  <<
                save.animation.speed <<save.animation.ke <<save.animation.ake <<save.animation.pe<<
                save.simulation.speed<<save.simulation.ke<<save.simulation.ake<<save.animation.pe<<
                save.ke_diff         <<save.ake_diff     <<save.pe_diff;
         parser.nextLine();
    }
    parser.saveInFile(name);
}

void HumanBody::saveFullDataList(const SimulationParameters& params){
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    part_info save;
    name = name +"_output_full";
    CSVParser parser;
    parser<<"id"<<
            "ECT animation (J)" <<"ECA animation (J)" <<"EC totale animation"<<"EP animation (J)" <<
            "ECT simulation (J)"<<"ECA simulation (J)"<<"EC totale simulation"<<"EP simulation (J)"<<
            "ECT difference (J)"<<"ECA difference (J)"<<"EC totale difference"<<"EP difference (J)"<<"duree(ms):"<<params.get_duration()<<"pas(ms):"<<params.get_steps_duration();
    parser.nextLine();
    for (int i = 0; i < _full_data_list.size(); ++i) {
        save=_full_data_list.at(i);
        parser<<i<<
        save.animation.speed<<save.animation.pt_aspeed.x<<save.animation.pt_aspeed.y<<save.animation.pt_aspeed.z<<
        save.animation.ke <<save.animation.ake <<save.animation.ake+save.animation.ke  <<save.animation.pe <<
        save.simulation.ke<<save.simulation.ake<<save.simulation.ake+save.simulation.ke<<save.simulation.pe<<
        save.ke_diff      <<save.ake_diff      <<save.ake_diff+save.ake_diff           <<save.pe_diff;
        parser.nextLine();
    }
    parser.saveInFile(name);

}
void HumanBody::updateInformationJointTree(float elapsed, float diff,float gravity,JointNode* node, btTransform transform){
    if (node == NULL){
        node = _joints_tree.get_root();
        transform.setIdentity();
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
        if (node) setSimulationPositionJointTree(elapsed,node,transform);
        else qWarning()<<"No root in part tree";
    } else {
        btTransform object_transform = node->get_data()->get_animation().getWorldTransform(transform,elapsed);
        Joint * data= node->get_data();
        if (elapsed == 0.0f){
            if(data->get_main_part() != NULL)
                data->get_main_part()->setInitialPosition(object_transform,transform);
        }
        else if(data->get_main_part() != NULL) data->get_main_part()->setSimulationPosition(object_transform,elapsed);
        for (int i = 0; i < node->get_number_of_children(); ++i) {
            setSimulationPositionJointTree(elapsed,node->childAt(i),object_transform);
        }
    }
}
void HumanBody::buildJointTree(){
    Joint * temp;
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
                    qDebug()<<"part :"<<temp->_part_name;
                    qDebug()<<"parent :"<<temp->_parent_part_name;
                    if (parent!=NULL){
                        _joints_tree.addNode(temp->_part_name,temp,parent->get_id());
                        Part * new_object = new Part();
                        temp->set_main_part(new_object);
                        new_object->set_mass(1);
//                        new_object->set_mass(temp->_part_mass);
                        new_object->set_body_part(temp->_part_name);
                        new_object->get_shape_struct().set_shape_type(Shape::capsule);
                        new_object->set_animated(true);
                        btVector3 extends = btVector3(temp->get_animation().extremityTranslationVector(0));
                        qDebug()<<extends.length();
                        new_object->get_shape_struct().set_shape(btVector3(.05,extends.length(),.05));
                        new_object->buildMesh();
                        _limbs.append(new_object);
                        inserted[j] = true;
                    }
                }
            }
        }
        complete = true;
        for (int i = 0; i < number_of_joints; ++i) {
            if (inserted[i] == false) complete = false;
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
                qDebug()<<list.at(i).first->get_data()->_part_name<<" "<<list.at(i).second->get_data()->_part_name;

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
