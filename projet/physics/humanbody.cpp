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
        delete _constraints[i].get_constraint();
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
    qDebug()<<path;
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
                        if ((BodyInfo::mass(temp.at(1),_mass)==0)){
                            qWarning()<<"Object mass null for part: "<<temp.at(1);
                        }
                        object->set_mass(BodyInfo::mass(temp.at(1),_mass));
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
    qDebug()<<list2.size();
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
}

void HumanBody::recordStatus(){
    part_info full_data;
    full_data.simulation.ake = 0;
    full_data.simulation.ke = 0;
    full_data.simulation.ake = 0;

    full_data.animation.speed = 0;
    full_data.animation.ake = 0;
    full_data.animation.ke = 0;
    full_data.animation.ake = 0;

    full_data.ake_diff = 0;
    full_data.ke_diff = 0;
    full_data.pe_diff = 0;

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
    QString path = "output/";
    QChar c = ',';
    QChar nl = '\n';
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output_"+part_name;
    QString oldname=name;
    QString ext="csv";

    part_info save;
    QFile file;
    file.setFileName(name+"."+ext);
    int i = 1;
    if (QDir::setCurrent(path))
        qDebug()<<"path set";
    else {
        qDebug()<<"path not set "<<path;
    }
    while (file.exists()){
        qWarning()<<"File already exists";
        name = oldname+" ("+QString::number(i)+")";
        file.setFileName(name+"."+ext);
        ++i;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning()<<"Couldn't write file "<<path;
        exit(0);
    } else {
        QTextStream stream(&file);
        stream<<"id"<<c<<"x animation"<<c<<"y animation"<<c<<"z animation"<<c<<"x simulation"<<c<<" y simulation"<<c<<"z simulation"<<c<<
                "vitesse animation" <<c<<"EC animation"  <<c<<"ECA animation" <<c<<"EP animation" <<c<<
                "vitesse simulation"<<c<<"EC simulation" <<c<<"ECA simulation"<<c<<"EP simulation"<<c<<
                "EC difference"     <<c<<"ECA difference"<<c<<"EP difference" <<c<<"erreur"       <<nl;
        for (int i = 0; i < _data_list.size(); ++i) {
            if (_data_list[i].part_name == part_name){
                save=_data_list.at(i);
                stream<<save.part_name<<c<<
                        save.animation.x     <<c<<save.animation.y  <<c<<save.animation.z   <<c<<
                        save.simulation.x    <<c<<save.simulation.y <<c<<save.simulation.z  <<c<<
                        save.animation.speed <<c<<save.animation.ke <<c<<save.animation.ake <<c<<save.animation.pe<<c<<
                        save.simulation.speed<<c<<save.simulation.ke<<c<<save.simulation.ake<<c<<save.animation.pe<<c<<
                        save.ke_diff         <<c<<save.ake_diff     <<c<<save.pe_diff       <<nl;
            }
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}


void HumanBody::saveCompleteDataList() const{
    QString path = "output/";
    QChar c = ',';
    QChar nl = '\n';
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output_complete";
    QString oldname=name;
    QString ext="csv";

    part_info save;
    QFile file;
    file.setFileName(name+"."+ext);
    int i = 1;
    if (QDir::setCurrent(path))
        qDebug()<<"path set";
    else {
        qDebug()<<"path not set "<<path;
    }
    while (file.exists()){
        qWarning()<<"File already exists";
        name = oldname+" ("+QString::number(i)+")";
        file.setFileName(name+"."+ext);
        ++i;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning()<<"Couldn't write file "<<path;
        exit(0);
    } else {
        QTextStream stream(&file);
        stream<<"id"<<c<<"x animation"<<c<<"y animation"<<c<<"z animation"<<c<<"x simulation"<<c<<" y simulation"<<c<<"z simulation"<<c<<
                "vitesse animation" <<c<<"EC animation"  <<c<<"ECA animation" <<c<<"EP animation" <<c<<
                "vitesse simulation"<<c<<"EC simulation" <<c<<"ECA simulation"<<c<<"EP simulation"<<c<<
                "EC difference"     <<c<<"ECA difference"<<c<<"EP difference" <<c<<"erreur"       <<nl;
        for (int i = 0; i <  _complete_data_list.size(); ++i) {
            save=_complete_data_list.at(i);
            stream<<save.part_name<<c<<
                    save.animation.x     <<c<<save.animation.y  <<c<<save.animation.z   <<c<<
                    save.simulation.x    <<c<<save.simulation.y <<c<<save.simulation.z  <<c<<
                    save.animation.speed <<c<<save.animation.ke <<c<<save.animation.ake <<c<<save.animation.pe<<c<<
                    save.simulation.speed<<c<<save.simulation.ke<<c<<save.simulation.ake<<c<<save.animation.pe<<c<<
                    save.ke_diff         <<c<<save.ake_diff     <<c<<save.pe_diff       <<nl;
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}

void HumanBody::saveFullDataList(const SimulationParameters& params){
    QString path = "output/";
    QChar c(',');
    QChar nl('\n');
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output_full";
    QString oldname=name;
    QString ext="csv";

    part_info save;
    QFile file;
    file.setFileName(name+"."+ext);
    int i = 1;
    if (QDir::setCurrent(path))
        qDebug()<<"path set";
    else {
        qDebug()<<"path not set "<<path;
    }
    while (file.exists()){
        qWarning()<<"File already exists";
        name = oldname+" ("+QString::number(i)+")";
        file.setFileName(name+"."+ext);
        ++i;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning()<<"Couldn't write file "<<path;
        exit(0);
    } else {
        QTextStream stream(&file);
        stream<<"id,"<<
                "ECT animation (J)" <<c<<"ECA animation (J)" <<c<<"EC totale animation"<<c<<"EP animation (J)" <<c<<
                "ECT simulation (J)"<<c<<"ECA simulation (J)"<<c<<"EC totale simulation"<<c<<"EP simulation (J)"<<c<<
                "ECT difference (J)"<<c<<"ECA difference (J)"<<c<<"EC totale difference"<<c<<"EP difference (J)"<<c<<"duree(ms):"<<c<<params.get_duration()<<c<<"pas(ms):"<<c<<params.get_steps_duration()<<nl;
        for (int i = 0; i < _full_data_list.size(); ++i) {
            save=_full_data_list.at(i);
            stream<<i<<c<<
            save.animation.speed<<c<<save.animation.pt_aspeed.x<<c<<save.animation.pt_aspeed.y<<c<<save.animation.pt_aspeed.z<<c<<
            save.animation.ke <<c<<save.animation.ake <<c<<save.animation.ake+save.animation.ke  <<c<<save.animation.pe <<c<<
            save.simulation.ke<<c<<save.simulation.ake<<c<<save.simulation.ake+save.simulation.ke<<c<<save.simulation.pe<<c<<
            save.ke_diff      <<c<<save.ake_diff      <<c<<save.ake_diff+save.ake_diff           <<c<<save.pe_diff      <<nl;
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}

void HumanBody::exportSimulationToAnimation(){
    QString path = "output/";
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output_animation";
    QString oldname=name;
    QString ext="csv";
    QFile file;
    file.setFileName(name+"."+ext);
    int i = 1;
    if (QDir::setCurrent(path))
        qDebug()<<"path set";
    else {
        qDebug()<<"path not set "<<path;
    }
    while (file.exists()){
        qWarning()<<"File already exists";
        name = oldname+" ("+QString::number(i)+")";
        file.setFileName(name+"."+ext);
        ++i;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning()<<"Couldn't write file "<<path;
        exit(0);
    } else {
        QTextStream stream(&file);
        QChar c(';');
        QChar ln('\n');
        stream<<"body"<<c<<_mass<<c<<ln;
        for (int i = 0; i < _parts.size(); ++i) {
            stream << _parts[i]->exportSimulationToAnimation();
        }
        stream<<"parts_end"<<c<<ln;
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}

void HumanBody::updateBodyInformations(float elapsed,float diff,float gravity){
    for (int i = 0; i < _parts.size(); ++i){
        _parts[i]->updatePartInfo(elapsed,diff,gravity);
        _complete_data_list.append(_parts[i]->getEnergyInformation());
    }
}

void HumanBody::setSimulationPosition(float time){
    for (int i = 0; i < _parts.size(); ++i)
        _parts[i]->setSimulationPosition(time);
}

void HumanBody::buildTree(){
    for (int i = 0; i < _parts.size(); ++i) {

    }
}
