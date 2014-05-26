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
#include "bodyinfo.h"

float BodyInfo::mass(const QString& body_part, float total_mass){
    float proportion;
    static QHash<QString,float> segments;
    static bool first = true;
    if (first){

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
    }

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
