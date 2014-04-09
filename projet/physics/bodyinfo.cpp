#include "bodyinfo.h"

float BodyInfo::mass(const QString& body_part, float total_mass){
    float proportion;
    QHash<QString,float> segments;
//    segments.insert("head",0);
//    segments.insert("whole_trunk",55.1);
//    segments.insert("thorax",20.1);

//    segments.insert("abdomen",13.06);
//    segments.insert("pelvis",13.66);
//    segments.insert("total_arm",5.7);

//    segments.insert("upper_arm",3.25);
//    segments.insert("left_upper_arm",0);
//    segments.insert("right_upper_arm",3.25);

//    segments.insert("forearm",1.87);
//    segments.insert("left_forearm",1.87);
//    segments.insert("right_forearm",0);

//    segments.insert("hand",0.65);
//    segments.insert("left_hand",0.65);
//    segments.insert("right_hand",0.65);

//    segments.insert("forearm_&_hand",2.52);
//    segments.insert("total_leg",16.68);
//    segments.insert("thigh",10.5);

//    segments.insert("leg",4.75);
//    segments.insert("left_leg",4.75);
//    segments.insert("right_leg",4.75);

//    segments.insert("foot",1.43);
//    segments.insert("left_foot",1.43);
//    segments.insert("right_foot",1.43);

//    segments.insert("leg & foot",6.18);


    ////////////////////////////////////////////////////////////
    segments.insert("head",8.26);
    segments.insert("whole_trunk",55.1);
    segments.insert("thorax",20.1);

    segments.insert("abdomen",13.06);
    segments.insert("pelvis",13.66);
    segments.insert("total_arm",5.7);

    segments.insert("upper_arm",3.25);
    segments.insert("left_upper_arm",3.25);
    segments.insert("right_upper_arm",3.25);

    segments.insert("forearm",1.87);
    segments.insert("left_forearm",1.87);
    segments.insert("right_forearm",1.87);

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
    segments.insert("left_foot",1.43);
    segments.insert("right_foot",1.43);

    segments.insert("leg & foot",6.18);


    segments.insert("spine",20.1);//not good

    QHash<QString,float>::const_iterator it = segments.find(body_part.toLower());
    if (it !=segments.end())
        proportion = it.value();
    else proportion = 0;
//    return 0;
    return (total_mass/100) * proportion;
}

const CSVParser& BodyInfo::jointList(){
//    QList<QStringList> list;
    QStringList a;

//    a<<"thorax"<<"head";
//    list.append(a);
//    a.clear();
//    a<<"head"<<"pelvis";
//    list.append(a);
//    a.clear();
//    a<<"head"<<"none";
//    list.append(a);
//    a.clear();
//    a<<"thorax"<<"none";
//    list.append(a);
//    a.clear();
    static bool first_time = true;
    static CSVParser parser;
    if (first_time){
        parser.parseFile(":/CSV/input/jointvalues.csv",";");

        first_time = false;
    }
    return parser;
}

//QList<QPair<QString,QString> > BodyInfo::angularCalculationList(){
//    QList<QPair<QString,QString> > list;
//    list.append(QPair<QString,QString>("thorax","head"));
//    list.append(QPair<QString,QString>("head","pelvis"));
//    list.append(QPair<QString,QString>("head","none"));
//    list.append(QPair<QString,QString>("thorax","none"));
//    return list;
//}
