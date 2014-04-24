#include "humanbody.h"

HumanBody::HumanBody():
    _mass(0)
{
}

HumanBody::~HumanBody(){
    for (int i = 0; i < _parts.size(); ++i) {
        delete _parts[i];
    }
}

void HumanBody::loadObjects(QString path){
    QString filename=path;
    CSVParser list;
    list.parseFile(":/CSV/input/"+filename,";");
    QStringList temp;
    int i=0;
    InteractiveObject * object = NULL;
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
                        object = new InteractiveObject();
                        object->get_shape_struct().set_shape_type(Shape::capsule);
                        object->set_body_part(temp.at(1));
                        mass = BodyInfo::mass(temp.at(1),_mass);
                        if (temp.size() == 5 ){
                            if (temp.at(4) == "zeromass") mass = 0.0f;
                            QString parent = temp.at(3);
                            object->set_parent_body_part(parent);
                        }
                        if (mass==0.0f){
                            qWarning()<<"Object mass null for part: "<<temp.at(1);
                        }
                        object->set_mass(BodyInfo::mass(temp.at(1),mass));
                    }
                }
                else if (!ignore){
                    if (temp.at(0)=="scaling") {
                        for (int k=0; k<2;k++) {
                            QStringList values = list[i+1+k] ;
                            for (int j=1; j<values.size()-1;j+=2){
                                object->get_animation().get_scaling_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                            }
                        }
                    } else if (temp.at(0)=="translation") {
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
        if (!ignore){
            _parts.append(object);
            object->set_local_inertia(btVector3(0,0,0));
            object->set_animated(true);
        }
        i++;
    }
    ignore = false;

    const CSVParser& list2 = BodyInfo::jointList();
    #ifdef DEECORE
    if (GlobalConfig::is_enabled("constraints_activated")){
    #endif
        for (int i = 0; i < list2.size(); ++i) {
            const QStringList& strlist = list2.at(i);;

            QList<InteractiveObject *>::iterator part1 = findPartByName(strlist.at(0));
            QList<InteractiveObject *>::iterator part2 = findPartByName(strlist.at(1));
            if (part1 != _parts.end() && ( part2 != _parts.end() || strlist.at(1) == "none")){
                Joint joint;
                joint._parts.first = *part1;
                if (part2 != _parts.end())
                    joint._parts.second= *part2;
                else joint._parts.second=NULL;

                if (strlist.size() > 2){
                    qDebug()<<strlist.size();
                    int n=3;
                    if (strlist.at(2) == "hinge")
                        joint._type=Joint::hinge;
                    else if (strlist.at(2) == "cone")
                        joint._type=Joint::cone;
                    else if (strlist.at(2) == "point")
                        joint._type=Joint::point;
                    btScalar vals[3];
                    bool good=false;

                    for (int id = 0; id < 3; ++id) {
                        vals[id] = strlist.at(id+3).toFloat(&good);
                        if (good == false ) qWarning()<<"Cannot convert str to bool";
                    }
                    joint._pivotA=btVector3(vals[0],vals[1],vals[2]);
                    joint._localeA.setOrigin(joint._pivotA);

                    for (int id = 0; id < 3; ++id) {
                        vals[id] = strlist.at(id+6).toFloat(&good);
                        if (good == false ) qWarning()<<"Cannot convert str to bool";
                    }
                    joint._pivotB=btVector3(vals[0],vals[1],vals[2]);
                    joint._localeB.setOrigin(joint._pivotB);

                    for (int id = 0; id < 3; ++id) {
                        qDebug()<<strlist.size();
                        vals[id] = strlist.at(id+9).toFloat(&good);
                        if (good == false ) qWarning()<<"Cannot convert str to bool";
                    }
                    joint._localeA.getBasis().setEulerZYX(deg2rad(vals[0]),deg2rad(vals[1]),deg2rad(vals[2]));

                    for (int id = 0; id < 3; ++id) {
                        qDebug()<<strlist.size();
                        vals[id] = strlist.at(id+12).toFloat(&good);
                        if (good == false ) qWarning()<<"Cannot convert str to bool";
                    }
                    joint._localeB.getBasis().setEulerZYX(deg2rad(vals[0]),deg2rad(vals[1]),deg2rad(vals[2]));

                    joint._complete =true;

                }
                if(strlist.at(15)!="ignore")
                    _constraints.append(joint);
            } else {
                qDebug()<<"parts not found for constraint";
            }

        }
    #ifdef DEECORE
    }
    #endif

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
    btTransform object_transform = node->get_data()->_animation.getWorldBaseTransform(transform,elapsed);
    node->get_data()->updatePartInfo(elapsed,diff,gravity,object_transform );
    for (int i = 0; i < node->get_number_of_children(); ++i) {
        updateInformationTree(node->childAt(i),object_transform ,elapsed,diff,gravity);
    }
    _complete_data_list.append(node->get_data()->getEnergyInformation());
}

void HumanBody::setSimulationPositionTree(const PartNode* node, const btTransform& transform, float elapsed){
    btTransform object_transform = node->get_data()->_animation.getWorldBaseTransform(transform,elapsed);
    if (elapsed == 0.0f)
        node->get_data()->setInitialPosition(object_transform);
    else node->get_data()->setSimulationPosition2(object_transform,elapsed);
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
