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
        delete _constraints[i]._constraint;
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
                        object->set_shape_type(InteractiveObject::cylinder);
                        object->set_body_part(temp.at(1));
//                        object->set_mass(10);
                        object->set_mass(BodyInfo::mass(temp.at(1),_mass));
                    }
                }
                else if (!ignore){
                    if (temp.at(0)=="scaling") {
                        if (!ignore) {
                            for (int k=0; k<3;k++) {
                                QStringList values = list[i+1+k] ;
                                for (int j=1; j<values.size()-1;j+=2){
                                    object->get_animation().get_scaling_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                                }
                            }
                        }
                    } else if (temp.at(0)=="translation") {
                        if (!ignore) {
                            for (int k=0; k<3;k++) {
                                QStringList values = list[i+1+k] ;
                                for (int j=1; j<values.size()-1;j+=2){
                                    object->get_animation().get_translation_curves()[k].insert(values[j].toFloat(),values[j+1].toFloat());
                                }
                            }
                        }
                    } else if (temp.at(0)=="rotation") {
                        if (!ignore) {
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
        }
        if (!ignore){
            _parts.append(object);
            object->set_local_inertia(btVector3(0,0,0));
            object->set_animated(true);
        }
        i++;
    }
    ignore = false;
//    while (list[i].at(0)!="constraints_end"){
//        temp= list[i];
//        if (temp.at(0) == "constraint"){
//            if (temp.at(1) == "ignore") ignore = true;
//            else ignore = false;

//            if (!ignore){
//                QList<InteractiveObject *>::iterator part1 = findPartByName(temp.at(2));
//                QList<InteractiveObject *>::iterator part2 = findPartByName(temp.at(3));
//                if (part1 != _parts.end() && part2 != _parts.end()){
//                    qDebug()<<(*part1)->get_body_part();
//                    qDebug()<<(*part2)->get_body_part();
//                    Joint joint;
//                    joint._parts.first = *part1;
//                    joint._parts.second= *part2;

//                    _constraints.append(joint);
//                    Joint joint2;
//                    joint2._parts.first = *part1;
//                    _constraints.append(joint2);

//                }
//            }
//        }
//        ++i;
//    }

    QList<QPair<QString,QString> > list2 = BodyInfo::jointList();
    for (int i = 0; i < list2.size(); ++i) {
        QList<InteractiveObject *>::iterator part1 = findPartByName(list2.at(i).first);
        QList<InteractiveObject *>::iterator part2 = findPartByName(list2.at(i).second);
        if (part1 != _parts.end() && ( part2 != _parts.end() || list2.at(i).second == "none")){
            Joint joint;
            joint._parts.first = *part1;
            if (part2 != _parts.end())
                joint._parts.second= *part2;
            else joint._parts.second=NULL;
            _constraints.append(joint);
        }

    }
}

void HumanBody::recordStatus(){
    InteractiveObject::part_info full_data;
    full_data.simulation.ake = 0;
    full_data.simulation.ke = 0;
    full_data.simulation.ake = 0;

    full_data.animation.ake = 0;
    full_data.animation.ke = 0;
    full_data.animation.ake = 0;

    full_data.ake_diff = 0;
    full_data.ke_diff = 0;
    full_data.pe_diff = 0;

    for (int i = 0; i < _parts.size(); ++i) {
        InteractiveObject * object = _parts[i];
        InteractiveObject::part_info energy = object->getEnergyInformation();
//        btScalar work = computeWork( energy.simulation.ke , energy.animation.ke , energy.simulation.ake , energy.animation.ake,  energy.simulation.pe, energy.animation.pe);
//        energy.ake_diff = work;
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
    QString path = "output/";
#ifdef DEECORE
    QString name=GlobalConfig::get_date_time()+"_output";
#else
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output";
#endif
    QString oldname=name;
    QString ext="csv";

    InteractiveObject::part_info save;
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
        stream<<"id,x animation,y animation,z animation,x simulation, y simulation,z simulation,"<<
                "vitesse animation,EC animation,ECA animation,EP animation,"<<
                "vitesse simulation,EC simulation,ECA simulation,EP simulation,"<<
                "EC difference,ECA difference,EP difference,erreur\n";
        for (int i = 0; i < _data_list.size(); ++i) {
            save=_data_list.at(i);
            stream<<save.part_name<<","<<
                    save.animation.x<<","<<save.animation.y<<","<<save.animation.z<<","<<
                    save.simulation.x<<","<<save.simulation.y<<","<<save.simulation.z<<","<<
                    save.animation.speed<<","<<save.animation.ke<<","<<save.animation.ake<<","<<save.animation.pe<<","<<
                    save.simulation.speed<<","<<save.simulation.ke<<","<<save.simulation.ake<<","<<save.animation.pe<<","<<
                    save.ke_diff<<","<<save.ake_diff<<","<<save.pe_diff<<","<<save.mean_error<<"\n";
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}

void HumanBody::saveFullDataList(){
    QString path = "output/";
#ifdef DEECORE
    QString name=GlobalConfig::get_date_time()+"_output_full";
#else
    QString name=QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");
    name = name +"_output";
#endif
    QString oldname=name;
    QString ext="csv";

    InteractiveObject::part_info save;
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
                "EC animation,ECA animation,EP animation,"<<
                "EC simulation,ECA simulation,EP simulation,"<<
                "EC difference,ECA difference,EP difference,erreur\n";
        for (int i = 0; i < _full_data_list.size(); ++i) {
            save=_full_data_list.at(i);
            stream<<i<<","<<
                    save.animation.ke<<","<<save.animation.ake<<","<<save.animation.pe<<","<<
                    save.simulation.ke<<","<<save.simulation.ake<<","<<save.simulation.pe<<","<<
                    save.ke_diff<<","<<save.ake_diff<<","<<save.pe_diff<<"\n";
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }
}

void HumanBody::updateBodyInformations(float elapsed,float diff,float gravity){
    for (int i = 0; i < _parts.size(); ++i)
        _parts[i]->updatePartInfo(elapsed,diff,gravity);
}

void HumanBody::setSimulationPosition(float time){
    for (int i = 0; i < _parts.size(); ++i)
        _parts[i]->setSimulationPosition(time);
}
