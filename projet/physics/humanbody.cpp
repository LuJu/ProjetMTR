#include "humanbody.h"

HumanBody::HumanBody():
    _mass(0)
{
}

HumanBody::~HumanBody(){
    for (int i = 0; i < _parts.size(); ++i) {
        delete _parts[i];
    }
    for (int i = 0; i < _constraints.size(); ++i) {
        delete _constraints[i];
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
                        object->_parts.second= NULL;
                        object->_parts.first= NULL;
//                        qDebug()<<temp.size();
                        if (temp.size() >= 5 ){
                            if (temp.at(4) == "zeromass") mass = 0.0f;
                            QString parent = temp.at(3);
                            QString name = temp.at(1);
                            object->_part_name = name;
                            object->_parent_part_name = parent;
                            QList<Joint*>::iterator joint_parent = findJointByPartName(parent);
                            QList<InteractiveObject *>::iterator part_child = findPartByName(name);
                            QList<InteractiveObject *>::iterator part_parent = findPartByName(parent);
                            if (part_child == _parts.end() && joint_parent != _constraints.end()){
                                if (!ignore){
                                    InteractiveObject * new_object = new InteractiveObject();
                                    object->_parts.first= new_object;
                                    if (part_parent == _parts.end() ) {
                                        object->_parts.second= NULL;
                                        new_object->set_parent_body_part("root");
                                    } else {
                                        object->_parts.second= *part_parent;
                                        new_object->set_parent_body_part(parent);
                                    }
                                    new_object->get_shape_struct().set_shape_type(Shape::capsule);
                                    new_object->set_body_part(temp.at(1));
                                    new_object->set_animated(true);
                                    mass = BodyInfo::mass(temp.at(1),_mass);
                                    if (mass==0.0f){
                                        qWarning()<<"Object mass null for part: "<<temp.at(1);
                                        mass=1;
                                    }
                                    new_object->set_mass(mass);
                                    new_object->_animation = (*joint_parent)->_animation;
                                    new_object->_simulation= (*joint_parent)->_simulation;
                                    new_object->_animation.set_shape( &(new_object->_shape));
                                    new_object->_simulation.set_shape(&(new_object->_shape));
                                    _parts.append(new_object);

                                    if (temp.size() >= 6){
        //                                object->set_center_of_mass_proportion(temp.at(5).toFloat());
                                        if (temp.size() >= 7){
        //                                    mass=temp.at(6).toFloat();
                                            if (temp.size() >= 8){
                                                    if (temp.at(7) == "cone"){
                                                        object->_type=Joint::cone;
                                                    } else if (temp.at(7) == "hinge"){
                                                        object->_type=Joint::hinge;
                                                    } else {
                                                        object->_type=Joint::point;
                                                    }
                                            }
                                        }
                                    }
                                }
                            ///////////////////
                        } else {
                            qWarning()<<"Incorrect CSV file structure";
                        }
//                        object->set_mass(BodyInfo::mass(temp.at(1),mass));
                    }
                }
                } else if (!ignore) {
                    if (temp.at(0)=="scaling") {
                        for (int k=0; k<2;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                if (k == 0){
                                    object->_animation.get_scaling_curves()[k].insert(0.0f,0.03f);}
                                if (k == 1){
                                    object->_animation.get_scaling_curves()[k].insert(0.0f,values[j].toFloat());
                                    qDebug()<<values[j].toFloat()<<" "<<values[j+1].toFloat();}
                            }
                        }
                    } else if (temp.at(0)=="translation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                    object->_animation.get_translation_curves()[order[k]].insert(values[j].toFloat(),sign[0] * values[j+1].toFloat());
                            }
                        }
                    } else if (temp.at(0)=="rotation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                    object->_animation.get_rotation_curves()[order[k]].insert(values[j].toFloat(),sign[k] *values[j+1].toFloat());
                            }
                        }
                    }
                }
            }
        }
        if (!ignore){
            QString name;
            _constraints.append(object);
            qDebug()<<"llol "<<object->_part_name;
        }
        i++;
    }

    ignore = false;

//    QList<InteractiveObject *>::iterator part1 = _parts.begin();
//        for (int i = 0; i < _parts.size(); ++i) {
//            QList<InteractiveObject *>::iterator part2 = findPartByName((*part1)->get_parent_body_part());
//            if (part1 != _parts.end() && ( part2 != _parts.end())){
//                Joint joint;
//                joint._parts.first = *part1;
//                if (part2 != _parts.end())
//                    joint._parts.second= *part2;
//                else joint._parts.second=NULL;

//                if ((*part1)->_joint_type == "cone"){
//                    joint._type=Joint::cone;
//                } else if ((*part1)->_joint_type == "hinge"){
//                    joint._type=Joint::hinge;
//                } else {
//                    joint._type=Joint::point;
//                }
//                _constraints.append(joint);
//            } else {
//#ifdef DEECORE
////            if (GlobalConfig::is_enabled("constraints_activated")){
////                qDebug()<<"parts not found for constraint";
////                if ((*part1)->get_parent_body_part() == "root"){
////                    Joint joint;
////                    joint._parts.first = *part1;
////                    joint._parts.second=NULL;
////                    _constraints.append(joint);
////                }
////            }
//#endif
//            }
//            part1++;
//        }

    for (int i = 0; i < _parts.size(); ++i) {
        _parts[i]->buildMesh();
    }
    buildTree();
}



void HumanBody::loadObjectsOLD(QString path){

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
    InteractiveObject * object = NULL;
    bool ignore = false;
    float mass;
    qDebug()<<"parsing file "<<path;
    while (list[i].at(0)!="parts_end"){
        if (i > 1040){
//            qDebug()<<"lol";
        }
        for (i; list[i].at(0)!="end" && i<list.size() ; ++i) {
            temp= list[i];
//            qDebug()<<temp.at(0) ;
            if (temp.size() > 0 ){
                if (temp.at(0) == "object") {
                    if (temp.at(2) == "ignore") ignore = true;
                    else ignore = false;
                    if (!ignore){
                        object = new InteractiveObject();
                        object->get_shape_struct().set_shape_type(Shape::capsule);
                        object->set_body_part(temp.at(1));
                        mass = BodyInfo::mass(temp.at(1),_mass);
//                        qDebug()<<temp.size();
                        if (temp.size() >= 5 ){
                            if (temp.at(4) == "zeromass") mass = 0.0f;
                            QString parent = temp.at(3);
                            object->set_parent_body_part(parent);
                            if (temp.size() >= 6){
                                object->set_center_of_mass_proportion(temp.at(5).toFloat());
                                if (temp.size() >= 7){
                                    mass=temp.at(6).toFloat();
                                    if (temp.size() >= 8){
                                        object->_joint_type = temp.at(7);
                                    }
                                }
                            }
                        } else {
                            qWarning()<<"Incorrect CSV file structure";
                        }
                        if (mass==0.0f){
                            qWarning()<<"Object mass null for part: "<<temp.at(1);
                            mass=1;
                        }
//                        object->set_mass(BodyInfo::mass(temp.at(1),mass));
                        object->set_mass(mass);
                    }
                }
                else if (!ignore){
                    if (temp.at(0)=="scaling") {
                        for (int k=0; k<2;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                if (k == 0){
//                                    qDebug()<<values[j].toFloat()<<" "<<values[j+1].toFloat();
                                    object->get_animation().get_scaling_curves()[k].insert(0.0f,0.03f);}
//                                object->get_animation().get_scaling_curves()[k].insert(0.0f,values[j].toFloat());}
                                if (k == 1){
                                    object->get_animation().get_scaling_curves()[k].insert(0.0f,values[j].toFloat());
//                                    object->get_animation().get_scaling_curves()[k].insert(0.0f,1.f);
                                    qDebug()<<values[j].toFloat()<<" "<<values[j+1].toFloat();}
//                                if (k == 0){
//                                    qDebug()<<values[j].toFloat()<<" "<<values[j+1].toFloat();
//                                    object->get_animation().get_scaling_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());}
//                                if (k == 1){
//                                    object->get_animation().get_scaling_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
//                                    qDebug()<<values[j].toFloat()<<" "<<values[j+1].toFloat();}
//                                qDebug();
                            }
                        }
                    } else if (temp.at(0)=="translation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                qDebug()<<"ORDER : "<<order[0]<<" "<<order[1]<<" "<<order[2];
                                qDebug()<<"sign: "<<sign[0]<<" "<<sign[1]<<" "<<sign[2];
//                                object->get_animation().get_translation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                                if (k == 0){
                                    object->get_animation().get_translation_curves()[order[0]].insert(values[j].toFloat(),sign[0] * values[j+1].toFloat());
                                }
                                if (k == 1){
                                    object->get_animation().get_translation_curves()[order[1]].insert(values[j].toFloat(),sign[1] * values[j+1].toFloat());
                                }
                                if (k == 2){
                                    object->get_animation().get_translation_curves()[order[2]].insert(values[j].toFloat(),sign[2] * values[j+1].toFloat());
                                }
                            }
                        }
                    } else if (temp.at(0)=="rotation") {
                        for (int k=0; k<3;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
//                                object->get_animation().get_rotation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                                if (k == 0){
                                    object->get_animation().get_rotation_curves()[order[0]].insert(values[j].toFloat(),sign[0] *values[j+1].toFloat());
                                }
                                if (k == 1){
                                    object->get_animation().get_rotation_curves()[order[1]].insert(values[j].toFloat(),sign[1] *values[j+1].toFloat());
                                }
                                if (k == 2){
                                    object->get_animation().get_rotation_curves()[order[2]].insert(values[j].toFloat(),sign[2] *values[j+1].toFloat());
                                }
//                                object->get_animation().get_rotation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                            }
                        }
                    }
                }
            }
        }
        if (!ignore){
            _parts.append(object);
            object->set_animated(true);
        }
        i++;
    }
    ignore = false;

//    QList<InteractiveObject *>::iterator part1 = _parts.begin();
//        for (int i = 0; i < _parts.size(); ++i) {
//            QList<InteractiveObject *>::iterator part2 = findPartByName((*part1)->get_parent_body_part());
//            if (part1 != _parts.end() && ( part2 != _parts.end())){
//                Joint joint;
//                joint._parts.first = *part1;
//                if (part2 != _parts.end())
//                    joint._parts.second= *part2;
//                else joint._parts.second=NULL;

//                if ((*part1)->_joint_type == "cone"){
//                    joint._type=Joint::cone;
//                } else if ((*part1)->_joint_type == "hinge"){
//                    joint._type=Joint::hinge;
//                } else {
//                    joint._type=Joint::point;
//                }
//                _constraints.append(joint);
//            } else {
//#ifdef DEECORE
////            if (GlobalConfig::is_enabled("constraints_activated")){
////                qDebug()<<"parts not found for constraint";
////                if ((*part1)->get_parent_body_part() == "root"){
////                    Joint joint;
////                    joint._parts.first = *part1;
////                    joint._parts.second=NULL;
////                    _constraints.append(joint);
////                }
////            }
//#endif
//            }
//            part1++;
//        }

    for (int i = 0; i < _parts.size(); ++i) {
        _parts[i]->buildMesh();
    }
    buildTree();
}



void HumanBody::recordStatus(){
    part_info full_data;
    for (int i = 0; i < _parts.size(); ++i) {
        InteractiveObject * object = _parts[i];
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
        for (int j = 0; j < _parts.size(); ++j) {
            savePartDataList(_parts[j]->get_body_part());
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

void HumanBody::updateBodyInformations(float elapsed,float diff,float gravity){
    btTransform transform;
    PartNode* root = _parts_tree.get_root();
    transform.setIdentity();
    if (root){
        updateInformationTree(root,transform,elapsed,diff,gravity);
    } else {
        qWarning()<<"No root in part tree";
    }
    for (int i = 0; i < _parts.size(); ++i){
        _complete_data_list.append(_parts[i]->getEnergyInformation());
    }
}

void HumanBody::updateInformationTree(const PartNode* node, const btTransform& transform, float elapsed, float diff,float gravity){
    btTransform object_transform = node->get_data()->_animation.getWorldTransform(transform,elapsed);
    node->get_data()->updatePartInfo(elapsed,diff,gravity,object_transform );
    for (int i = 0; i < node->get_number_of_children(); ++i) {
        updateInformationTree(node->childAt(i),object_transform ,elapsed,diff,gravity);
    }
    _complete_data_list.append(node->get_data()->getEnergyInformation());
}

void HumanBody::setSimulationPositionTree(const PartNode* node, const btTransform& transform, float elapsed){
    btTransform object_transform = node->get_data()->_animation.getWorldTransform(transform,elapsed);
    if (elapsed == 0.0f)
        node->get_data()->setInitialPosition(object_transform);
    else node->get_data()->setSimulationPosition(object_transform,elapsed);
    for (int i = 0; i < node->get_number_of_children(); ++i) {
        setSimulationPositionTree(node->childAt(i),object_transform ,elapsed);

    }
}

void HumanBody::setSimulationPosition(float time){
    btTransform transform;
    PartNode* root = _parts_tree.get_root();
    transform.setIdentity();
    if (root){
        setSimulationPositionTree(root,transform,time);
    } else {
        qWarning()<<"No root in part tree";
    }
//    setSimulationPositionTree(root,transform,time);
//    for (int i = 0; i < _parts.size(); ++i)
//        _parts[i]->setSimulationPosition(time);
}


void HumanBody::buildTree(){
    InteractiveObject * temp;
    PartNode * parent;
    QList<InteractiveObject*>::iterator it;
    int number_of_parts = _parts.size();
    it = findPartByParentName("root");
    bool inserted[number_of_parts];
    bool complete = false;
    for (int i = 0; i < _parts.size(); ++i) {
        inserted[i] = false;
    }
    qDebug()<<_parts.size();
    while ( complete == false){
        for (int j = 0; j < number_of_parts; ++j) {
            if(inserted[j] == false){
                temp = _parts[j];
                if (temp->get_parent_body_part() == "root"){
                    _parts_tree.addNode(temp->get_body_part(),temp);
                    inserted[j] = true;
                } else {
                    parent = _parts_tree.get_node_by_name(temp->get_parent_body_part());
                    if (parent!=NULL){
                        _parts_tree.addNode(temp->get_body_part(),temp,parent->get_id());
                        inserted[j] = true;
                    }
                }
            }
        }
        complete = true;
        for (int i = 0; i < number_of_parts; ++i) {
            if (inserted[i] == false) complete = false;
        }
    }
}

void HumanBody::buildJointTree(){
    Joint * temp;
    JointNode * parent;
    QList<Joint*>::iterator it;
    int number_of_joints = _constraints.size();
    it = findJointByParentPartName("root");
    bool inserted[number_of_joints];
    bool complete = false;
    for (int i = 0; i < _constraints.size(); ++i) {
        inserted[i] = false;
    }
    while ( complete == false){
        for (int j = 0; j < number_of_joints; ++j) {
            if(inserted[j] == false){
                temp = _constraints[j];
                if (!temp->has_parent()){
                    _joints_tree.addNode(temp->_part_name,temp);
                    inserted[j] = true;
                } else {
                    parent = _joints_tree.get_node_by_name(temp->_parent_part_name);
                    if (parent!=NULL){
                        _joints_tree.addNode(temp->_part_name,temp,parent->get_id());
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


QList<Joint*>::iterator HumanBody::findJointByPartName(const QString& name){
    QList<Joint*>::iterator i;
    Joint * value;
    for (i = _constraints.begin(); i != _constraints.end(); ++i) {
        value = *i;
        if (value->_part_name == name) return i;
    }
    return _constraints.end();
}

QList<Joint*>::iterator HumanBody::findJointByParentPartName(const QString& name){
    QList<Joint*>::iterator i;
    Joint * value;
    for (i = _constraints.begin(); i != _constraints.end(); ++i) {
        value = *i;
        if (value->has_parent())
            if (value->_parent_part_name == name) return i;
    }
    return _constraints.end();
}
