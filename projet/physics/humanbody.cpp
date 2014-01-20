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
    QString filename=path;
    CSVParser list;
    list.parseFile(":/CSV/input/"+filename,";");
    QStringList temp;
    int i=0;
    InteractiveObject * object = NULL;
    bool ignore = false;

    while (list[i].at(0)!="parts_end"){
        for (i; list[i].at(0)!="end" && i<list.size() ; ++i) {
            temp= list[i];
            if (temp.size() > 0 ){
                if (temp.at(0) == "object") {
                    if (temp.at(2) == "ignore") ignore = true;
                    else ignore = false;
                    if (!ignore){
                        object = new InteractiveObject();
                        object->set_shape_type(InteractiveObject::cube);
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
    while (list[i].at(0)!="constraints_end"){
        temp= list[i];
        if (temp.at(0) == "constraint"){
            if (temp.at(1) == "ignore") ignore = true;
            else ignore = false;

            if (!ignore){
                QList<InteractiveObject *>::iterator part1 = findPartByName(temp.at(2));
                QList<InteractiveObject *>::iterator part2 = findPartByName(temp.at(3));
                if (part1 != _parts.end() && part2 != _parts.end()){
                    btPoint2PointConstraint * constraint= new btPoint2PointConstraint((*part1)->get_body(),(*part1)->get_body(),btVector3(0,1,0),btVector3(0,-1,0));
                    _constraints.append(constraint);
                }
            }
        }
        ++i;
    }


}

void HumanBody::calculateWork(){
    for (int i = 0; i < _parts.size(); ++i) {
        InteractiveObject * object = _parts[i];
        InteractiveObject::part_info energy = object->getEnergyInformation();
        btScalar work = computeWork( energy.simulation.ke , energy.animation.ke , energy.simulation.ake , energy.animation.ake,  energy.simulation.pe, energy.animation.pe);
        energy.work = work;
        _data_list.append(energy);
    }
}

btScalar HumanBody::computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation){
    return (ke_simulation - ke_animation)+ (ake_simulation - ake_animation ) + (pe_simulation - pe_animation) ;
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
                "travail,erreur\n";
        for (int i = 0; i < _data_list.size(); ++i) {
            save=_data_list.at(i);
            stream<<save.part_name<<","<<
                    save.animation.x<<","<<save.animation.y<<","<<save.animation.z<<","<<
                    save.simulation.x<<","<<save.simulation.y<<","<<save.simulation.z<<","<<
                    save.animation.speed<<","<<save.animation.ke<<","<<save.animation.ake<<","<<save.animation.pe<<","<<
                    save.simulation.speed<<","<<save.simulation.ke<<","<<save.simulation.ake<<","<<save.animation.pe<<","<<
                    save.work<<","<<save.mean_error<<"\n";
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
