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
    list.parseFile(GlobalConfig::find_asset_path_qstring("CSV/input/"+filename),";");
    QStringList temp;
    int i=0;
    int total_mass=0;
    InteractiveObject * object = NULL;
    while (list[i].at(0)!="final"){
        object = new InteractiveObject();
        object->_shape_type=InteractiveObject::cylinder;

        for (i; list[i].at(0)!="end" && i<list.size() ; ++i) {
            temp= list[i];
            if (temp.size()>=1){
                if (temp.at(0)=="scaling") {
                    for (int k=0; k<3;k++) {
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
                } else if (temp.at(0) == "object") {
                    object->set_body_part(temp.at(1));
                    object->set_mass(BodyInfo::mass(temp.at(1),total_mass));
                } else if (temp.at(0) == "body") {
                    total_mass = temp.at(1).toFloat();
                }
            }
        }
        object->set_local_inertia(btVector3(0,0,0));
        object->set_animated(true);
        _parts.append(object);
        i++;
    }
}

void HumanBody::calculateWork(){
    for (int i = 0; i < _parts.size(); ++i) {
        InteractiveObject * object = _parts[i];
        InteractiveObject::energy_info energy = object->getEnergyInformation();
        btScalar work = computeWork( energy.simulation_ke , energy.animation_ke , energy.simulation_ake , energy.animation_ake,  energy.simulation_pe, energy.animation_pe);
        energy.work = work;
        _data_list.append(energy);
    }
}

btScalar HumanBody::computeWork(btScalar ke_simulation , btScalar ke_animation , btScalar ake_simulation , btScalar ake_animation , btScalar pe_simulation , btScalar pe_animation){
    return (ke_simulation - ke_animation)+ (ake_simulation - ake_animation ) + (pe_simulation - pe_animation) ;
}

void HumanBody::saveDataList(){
    QString path;
    InteractiveObject::energy_info save;
    path = QString("CSV/output/"+GlobalConfig::get_date_time()+"_output.csv");
    QFile file(GlobalConfig::find_asset_path_qstring(path));
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
                    save.animation_x<<","<<save.animation_y<<","<<save.animation_z<<","<<
                    save.simulation_x<<","<<save.simulation_y<<","<<save.simulation_z<<","<<
                    save.animation_speed<<","<<save.animation_ke<<","<<save.animation_ake<<","<<save.animation_pe<<","<<
                    save.simulation_speed<<","<<save.simulation_ke<<","<<save.simulation_ake<<","<<save.animation_pe<<","<<
                    save.work<<","<<save.mean_error<<"\n";
        }
        file.close();
        qDebug()<<"File successfully written : "<<path;
    }
}

void HumanBody::updateEnergyInformations(float elapsed,float diff,float gravity){
    for (int i = 0; i < _parts.size(); ++i)
        _parts[i]->update_energy_info(elapsed,diff,gravity);
}

void HumanBody::setSimulationPosition(float time){
    for (int i = 0; i < _parts.size(); ++i)
        _parts[i]->setSimulationPosition(time);
}
