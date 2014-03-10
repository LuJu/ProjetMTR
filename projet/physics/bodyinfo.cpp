#include "bodyinfo.h"

float BodyInfo::mass(const QString& body_part, float total_mass){
    float proportion;
    QHash<QString,float> segments;
    segments.insert("head",8.26);
    segments.insert("whole trunk",55.1);
    segments.insert("thorax",20.1);

    segments.insert("abdomen",13.06);
    segments.insert("pelvis",13.66);
    segments.insert("total arm",5.7);

    segments.insert("upper arm",3.25);
    segments.insert("left upper arm",3.25);
    segments.insert("right upper arm",3.25);

    segments.insert("forearm",1.87);
    segments.insert("left forearm",1.87);
    segments.insert("right forearm",1.87);

    segments.insert("hand",0.65);
    segments.insert("left_hand",0.65);
    segments.insert("right_hand",0.65);

    segments.insert("forearm_&_hand",2.52);
    segments.insert("total_leg",16.68);
    segments.insert("thigh",10.5);

    segments.insert("leg",4.75);
    segments.insert("left_leg",4.75);
    segments.insert("right_leg",4.75);

    segments.insert("foot",1.43);
    segments.insert("left foot",1.43);
    segments.insert("right foot",1.43);

    segments.insert("leg & foot",6.18);

    QHash<QString,float>::const_iterator it = segments.find(body_part.toLower());
    if (it !=segments.end())
        proportion = it.value();
    else proportion = 0;
    return (total_mass/100) * proportion;
}

QList<QStringList> BodyInfo::jointList(){
    QList<QStringList> list;
//    list.append(QStringList("thorax","head"));
//    list.append(QStringList("head","pelvis"));
//    list.append(QStringList("head","none"));
//    list.append(QStringList("thorax","none"));
    return list;
}

QList<QPair<QString,QString> > BodyInfo::angularCalculationList(){
    QList<QPair<QString,QString> > list;
    list.append(QPair<QString,QString>("thorax","head"));
    list.append(QPair<QString,QString>("head","pelvis"));
    list.append(QPair<QString,QString>("head","none"));
    list.append(QPair<QString,QString>("thorax","none"));
    return list;
}
