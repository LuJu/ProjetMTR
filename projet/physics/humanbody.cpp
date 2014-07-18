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
    BodyInfo::loadData(body_name);

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
                        } else {
                            if (temp.size() >= 5 && temp.at(4).contains("zero",Qt::CaseInsensitive))
                                mass = 0.0f;
                            else mass = 1.0f;
                            if (temp.size() >= 6){
                                    object->_part_com_proportion = temp.at(5).toFloat();
                                    mass=temp.at(6).toFloat();
                            }
                        }
                        if (temp.size() >= 7){
                            if (temp.at(6) == "cone") object->_constraint_type=Constraint::cone;
                            else if (temp.at(6) == "hinge") object->_constraint_type=Constraint::hinge;
                            else object->_constraint_type=Constraint::point;
                        }
                        object->_part_mass=mass;
                    }
                } else if (!ignore) {
                    if (temp.at(0)=="translation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                object->get_animation().get_translation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                            }
                        }
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
        if (!ignore)_joints.append(object);
        i++;
    }

    ignore = false;
    for (int i = 0; i < _limbs.size(); ++i) {
        _limbs[i]->buildMesh();
    }
    buildSceneGraph();
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
    _sum_data_list.append(full_data);
}


void HumanBody::saveDataList(){
    for (int j = 0; j < _limbs.size(); ++j) {
        savePartDataList(_limbs[j]->get_body_part());
    }
}

void HumanBody::recordSegmentData(){
    part_info segment_data;
    QStringList effective_segments = BodyInfo::getSegmentList();
    QString segment;
    float time = -1;

    for (int i = 0; i < effective_segments.size(); ++i) {
        segment = effective_segments.at(i);
        time = -1;
        for (int j = 0; j < _data_list.size(); ++j) {
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
        _segments_data_list.append(segment_data);
    }
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
                    save.simulation.speed<<save.simulation.ke<<save.simulation.ake<<save.simulation.pe<<
                    save.ke_diff         <<save.ake_diff     <<save.pe_diff;
            parser.nextLine();
        }
    }
    parser.saveInFile(name);



}


void HumanBody::saveSegmentsDataList(){
    QStringList effective_segments = BodyInfo::getSegmentList();
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
                save.simulation.speed<<save.simulation.ke<<save.simulation.ake<<save.simulation.pe<<
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
    for (int i = 0; i < _sum_data_list.size(); ++i) {
        save=_sum_data_list.at(i);
        parser<<i<<save.time<<
        save.animation.ke <<save.animation.ake <<save.animation.ake+save.animation.ke  <<save.animation.pe <<
        save.simulation.ke<<save.simulation.ake<<save.simulation.ake+save.simulation.ke<<save.simulation.pe<<
        save.ke_diff      <<save.ake_diff      <<save.ake_diff+save.ke_diff           <<save.pe_diff;
        parser.nextLine();
    }
    parser.saveInFile(name);

}
void HumanBody::updateInformation(float elapsed, float diff,float gravity,SceneGraphNode* node, btTransform transform){
    if (node == NULL){
        node = _scene_graph.get_root();
        transform.setIdentity();
#ifdef Y_AXIS_MINUS_90_DEGREES_ROTATION_OFFSET
        btQuaternion quat;
        quat.setEuler(0,-M_PI_2,0);
        transform.setRotation(quat);
#endif
        if (node){
            updateInformation(elapsed,diff,gravity,node,transform);
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
            updateInformation(elapsed,diff,gravity,node->childAt(i),object_transform);
        }
    }
}


void HumanBody::setSimulationPosition(float elapsed,SceneGraphNode* node, btTransform transform){
    if (node == NULL){
        node = _scene_graph.get_root(); // gets root if the function is called with no parameter
        transform.setIdentity();
#ifdef Y_AXIS_MINUS_90_DEGREES_ROTATION_OFFSET
        btQuaternion quat;
        quat.setEuler(0,-M_PI_2,0);
        transform.setRotation(quat);
#endif
        if (node) setSimulationPosition(elapsed,node,transform);
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
            setSimulationPosition(elapsed,node->childAt(i),object_transform);
        }
    }
}
void HumanBody::buildSceneGraph(){
    Joint * temp;
    Part * new_part;
    SceneGraphNode * parent;
    int number_of_joints = _joints.size();
    int uncomplete = number_of_joints;
    bool inserted[number_of_joints];// array of bools to check which nodes have been insterted

    for (int i = 0; i < number_of_joints; ++i) {
        inserted[i] = false;
    }

    while ( uncomplete > 0){
        for (int j = 0; j < number_of_joints; ++j) {
            if(inserted[j] == false){
                temp = _joints[j];
                if (!temp->has_parent()){
                    _scene_graph.addNode(temp->_part_name,temp);
                    inserted[j] = true;
                } else {
                    parent = _scene_graph.get_node_by_name(temp->_parent_part_name);
                    if (parent!=NULL){
                        _scene_graph.addNode(temp->_part_name,temp,parent->get_id());
                        new_part = new Part();
                        temp->set_main_part(new_part);
                        new_part->set_mass(temp->_part_mass);
                        new_part->set_body_part(temp->_part_name);
                        new_part->get_shape_struct().set_shape_type(Shape::capsule);
                        new_part->set_animated(true);
                        btVector3 extends = btVector3(temp->get_animation().TranslationVector(0));
                        new_part->get_shape_struct().set_shape(btVector3(.02,extends.length(),.02));
                        new_part->buildMesh();
                        _limbs.append(new_part);
                        inserted[j] = true;
                    }
                }
                uncomplete--;
            }
        }
    }
}

void HumanBody::buildConstraints(SceneGraphNode * current_node){
    Joint * temp;
    if (current_node == NULL){
        current_node = _scene_graph.get_root();
    }
    if (current_node != NULL){
        temp = current_node->get_data();
        QList<QPair<SceneGraphNode*,SceneGraphNode*> > list;
        int n = current_node->get_number_of_children();
        for (int i = 0; i < n; ++i) {
            list.append(QPair<SceneGraphNode*,SceneGraphNode*>(current_node,current_node->childAt(i)));
        }
        for (int i = 0; i < n-1; ++i) {
            for (int j = i; j < n-1 ; ++j) {
                list.append(QPair<SceneGraphNode*,SceneGraphNode*>(current_node->childAt(i),current_node->childAt(j+1)));
            }
        }
        for (int i = 0; i < list.size(); ++i) {
            if (list.at(i).first->get_data()->get_main_part()){
                if (list.at(i).first == current_node)
                    _constraints.append(Constraint(temp->_constraint_type,list.at(i).first->get_data()->get_main_part(),list.at(i).second->get_data()->get_main_part(),true));
                else
                    _constraints.append(Constraint(temp->_constraint_type,list.at(i).first->get_data()->get_main_part(),list.at(i).second->get_data()->get_main_part()));
            } else { // means that it is root
                if (GlobalConfig::is_enabled("root_fixed"))
                    _constraints.append(Constraint(Constraint::point,list.at(i).second->get_data()->get_main_part(),NULL,false));
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
