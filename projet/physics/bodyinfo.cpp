#include "bodyinfo.h"

BodyInfo::BodyInfo()
{
}


float BodyInfo::mass(QString body_part, float total_mass){
    float proportion;
    QHash<QString,float> segments;
    segments.insert("head",8.26);
    segments.insert("whole trunk",55.1);
    segments.insert("thorax",20.1);

    segments.insert("abdomen",13.06);
    segments.insert("pelvis",13.66);
    segments.insert("total arm",5.7);

    segments.insert("upper arm",3.25);
    segments.insert("forearm",1.87);
    segments.insert("hand",0.65);

    segments.insert("forearm & hand",2.52);
    segments.insert("total leg",16.68);
    segments.insert("thigh",10.5);
    segments.insert("leg",4.75);
    segments.insert("foot",1.43);
    segments.insert("leg & foot",6.18);

    QHash<QString,float>::Iterator it = segments.find(body_part.toLower());
    if (it !=segments.end())
        proportion = it.value();
    else proportion = 0;
    return (total_mass/100) * proportion;
}
