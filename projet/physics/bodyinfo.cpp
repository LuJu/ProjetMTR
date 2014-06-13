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

QHash<QString,bool> BodyInfo::_segments_fixed;
QList<QString> BodyInfo::_head_list;
QHash<QString,float> BodyInfo::_sub_segments;
QHash<QString,float> BodyInfo::_segments;
QHash<QString,QString> BodyInfo::_segments_parenting;

float BodyInfo::subSegmentMass(const QString& body_part, float total_mass){
    float proportion;
    if (_sub_segments.size() == 0){

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

        int value = 1;
        ////////////////////////////////////////////////////////////
        _sub_segments.insert("head",8.26);
        _sub_segments.insert("whole_trunk",55.1);
        _sub_segments.insert("thorax",20.1);

        _sub_segments.insert("abdomen",13.06);
        _sub_segments.insert("pelvis",13.66);
        _sub_segments.insert("total_arm",5.7);

        _sub_segments.insert("upper_arm",3.25);
        _sub_segments.insert("left_upper_arm",3.25);
        _sub_segments.insert("right_upper_arm",3.25);

        _sub_segments.insert("forearm",1.87);
        _sub_segments.insert("left_forearm",1.87);
        _sub_segments.insert("right_forearm",1.87);

        _sub_segments.insert("hand",0.65);
        _sub_segments.insert("left_hand",0.65);
        _sub_segments.insert("right_hand",0.65);

        _sub_segments.insert("forearm_&_hand",2.52);
        _sub_segments.insert("total_leg",16.68);
        _sub_segments.insert("thigh",10.5);

        _sub_segments.insert("leg",4.75);
        _sub_segments.insert("left_leg",4.75);
        _sub_segments.insert("right_leg",4.75);

        _sub_segments.insert("foot",1.43);
        _sub_segments.insert("left_foot",1.43);
        _sub_segments.insert("right_foot",1.43);

        _sub_segments.insert("leg & foot",6.18);


        _sub_segments.insert("Hips",value);

    //    segments.insert("Spine_Dummy_",value);
        _sub_segments.insert("Spine",13.66);

        _sub_segments.insert("Spine1",13.06);

        _sub_segments.insert("Spine2",20.1/3);
        _sub_segments.insert("LeftShoulder",20.1/3);
        _sub_segments.insert("RightShoulder",20.1/3);

        _sub_segments.insert("Neck",8.26/3);
        _sub_segments.insert("Head",8.26/3);
        _sub_segments.insert("Head_End",8.26/3);


    //    segments.insert("LeftShoulder_Dummy_",value);
    //    segments.insert("LeftArm_Dummy_",value);
        _sub_segments.insert("LeftArm",3.25/2);
        _sub_segments.insert("LeftArmRoll",3.25/2);

        _sub_segments.insert("LeftForeArm",1.87/2);
        _sub_segments.insert("LeftForeArmRoll",1.87/2);

        _sub_segments.insert("LeftHand",0.65/21);
        _sub_segments.insert("LeftHandThumb1",0.65/21);
        _sub_segments.insert("LeftHandThumb2",0.65/21);
        _sub_segments.insert("LeftHandThumb3",0.65/21);
        _sub_segments.insert("LeftHandThumb4",0.65/21);
        _sub_segments.insert("LeftHandIndex1",0.65/21);
        _sub_segments.insert("LeftHandIndex2",0.65/21);
        _sub_segments.insert("LeftHandIndex3",0.65/21);
        _sub_segments.insert("LeftHandIndex4",0.65/21);
        _sub_segments.insert("LeftHandMiddle1",0.65/21);
        _sub_segments.insert("LeftHandMiddle2",0.65/21);
        _sub_segments.insert("LeftHandMiddle3",0.65/21);
        _sub_segments.insert("LeftHandMiddle4",0.65/21);
        _sub_segments.insert("LeftHandRing1",0.65/21);
        _sub_segments.insert("LeftHandRing2",0.65/21);
        _sub_segments.insert("LeftHandRing3",0.65/21);
        _sub_segments.insert("LeftHandRing4",0.65/21);
        _sub_segments.insert("LeftHandPinky1",0.65/21);
        _sub_segments.insert("LeftHandPinky2",0.65/21);
        _sub_segments.insert("LeftHandPinky3",0.65/21);
        _sub_segments.insert("LeftHandPinky4",0.65/21);

    //    segments.insert("LeftArm_Dummy_",value);
    //    segments.insert("RightShoulder_Dummy_",value);
    //    segments.insert("RightArm_Dummy_",value);
        _sub_segments.insert("RightArm",3.25/2);
        _sub_segments.insert("RightArmRoll",3.25/2);

        _sub_segments.insert("RightForeArm",1.87/2);
        _sub_segments.insert("RightForeArmRoll",1.87/2);

        _sub_segments.insert("RightHand",0.65/21);
        _sub_segments.insert("RightHandThumb1",0.65/21);
        _sub_segments.insert("RightHandThumb2",0.65/21);
        _sub_segments.insert("RightHandThumb3",0.65/21);
        _sub_segments.insert("RightHandThumb4",0.65/21);
        _sub_segments.insert("RightHandIndex1",0.65/21);
        _sub_segments.insert("RightHandIndex2",0.65/21);
        _sub_segments.insert("RightHandIndex3",0.65/21);
        _sub_segments.insert("RightHandIndex4",0.65/21);
        _sub_segments.insert("RightHandMiddle1",0.65/21);
        _sub_segments.insert("RightHandMiddle2",0.65/21);
        _sub_segments.insert("RightHandMiddle3",0.65/21);
        _sub_segments.insert("RightHandMiddle4",0.65/21);
        _sub_segments.insert("RightHandRing1",0.65/21);
        _sub_segments.insert("RightHandRing2",0.65/21);
        _sub_segments.insert("RightHandRing3",0.65/21);
        _sub_segments.insert("RightHandRing4",0.65/21);
        _sub_segments.insert("RightHandPinky1",0.65/21);
        _sub_segments.insert("RightHandPinky2",0.65/21);
        _sub_segments.insert("RightHandPinky3",0.65/21);
        _sub_segments.insert("RightHandPinky4",0.65/21);

    //    segments.insert("RightArm_Dummy_",value);
    //    segments.insert("LeftShoulder_Dummy_",value);
    //    segments.insert("RightShoulder_Dummy_",value);
    //    segments.insert("LeftUpLeg_Dummy_",value);

        _sub_segments.insert("LeftUpLeg",10.5/2);
        _sub_segments.insert("LeftUpLegRoll",10.5/2);

        _sub_segments.insert("LeftLeg",4.75/2);
        _sub_segments.insert("LeftLegRoll",4.75/2);

        _sub_segments.insert("LeftFoot",1.43/3);
        _sub_segments.insert("LeftToeBase",1.43/3);
        _sub_segments.insert("LeftToes_End",1.43/3);

        _sub_segments.insert("RightFoot",1.43/3);
        _sub_segments.insert("RightToeBase",1.43/3);
        _sub_segments.insert("RightToes_End",1.43/3);
    //    segments.insert("RightUpLeg_Dummy_",value);

        _sub_segments.insert("RightUpLeg",4.75/2);
        _sub_segments.insert("RightUpLegRoll",4.75/2);

        _sub_segments.insert("RightLeg",10.5/2);
        _sub_segments.insert("RightLegRoll",10.5/2);



    //    segments.insert("LeftUpLeg_Dummy_",value);
    //    segments.insert("RightUpLeg_Dummy_",value);
    //    segments.insert("Spine_Dummy_",value);

    }
    QHash<QString,float>::const_iterator it = _sub_segments.find(body_part);
    if (it !=_sub_segments.end())
        proportion = it.value();
    else {
        qDebug()<<body_part;
        proportion = 0;
    }
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


QString BodyInfo::getSegment(const QString& sub_segment){
    CSVParser parser;
    QString filepath("../assets/CSV/input/segments_subs.csv");
    QString segment_name;
    QString sub_segment_name;

    if (_segments.size() == 0){
        parser.parseFile(filepath,",");
        if (parser.size() > 0){
            for (int i = 0; i < parser.size(); ++i) {
                sub_segment_name = parser.at(i).at(0).toLower().replace(" ","");
                segment_name =     parser.at(i).at(1).toLower().replace(" ","");
                _segments_parenting.insert(sub_segment_name,segment_name);
            }
        } else {
            qWarning()<<"Target file "<<filepath<<" not found";
        }
    }


    QHash<QString,QString>::const_iterator it = _segments_parenting.find(sub_segment.toLower().replace(" ",""));
    if (it !=_segments_parenting.end())
        segment_name = it.value();
    else segment_name=QString("");
    return segment_name;

}





float BodyInfo::segmentMass(const QString& body_part, float total_mass){
    CSVParser parser;
    QString filepath("../assets/CSV/input/segments_mass.csv");
    QString part_name;
    bool ok = false;
    float proportion;
    float part_mass;
    if (_segments.size() == 0){
        parser.parseFile(filepath,",");
        if (parser.size() > 0){
            for (int i = 0; i < parser.size(); ++i) {
                part_name = parser.at(i).at(0).toLower();
                part_mass = parser.at(i).at(1).toFloat(&ok);
                if (ok == false){
                    qWarning()<<"Error parsing file "<<filepath;
                }
                _segments.insert(part_name,part_mass);
            }
        } else {
            qWarning()<<"Target file "<<filepath<<" not found";
        }
    }


    QHash<QString,float>::const_iterator it = _segments.find(body_part);
    if (it !=_segments.end())
        proportion = it.value();
    else {
        proportion = 0;
    }
//    return 0;
    return (total_mass/100) * proportion;

}


void BodyInfo::genFixedInfo(const QString& filepath, QString target_name){
    CSVParser parser;
    QString part_name;
    bool fixed;
    parser.parseFile("../assets/CSV/input/"+filepath,",");
    int index = -1;
    qDebug()<<"target"<<target_name;
    if (parser.size() > 0){
        const QStringList& names = parser.at(0);
        for (int i = 1; i < names.size(); ++i) {
            qDebug()<<"Name : "<<target_name<<" "<<names.at(i);
            if (names.at(i) == target_name) {
                index = i;
                break;
            }
        }
        if (index != -1){
            for (int i = 1; i < parser.size(); ++i) {
                part_name = parser.at(i).at(0);
                if (parser.at(i).at(index).toLower() == "zero")
                    fixed = true;
                else fixed = false;

                _segments_fixed.insert(part_name, fixed);
            }
        } else {
            qWarning()<<"Target file "<<target_name<<" not found";
        }
    }
}

bool BodyInfo::isFixed(QString part_name){
    QHash<QString,bool>::const_iterator fixed = _segments_fixed.find(part_name);
    if (fixed != _segments_fixed.end()) {
        qDebug()<<"fixed " <<fixed.value();
        return fixed.value();
    }
    else
        qWarning()<<"Part "<<part_name<<" not found";
}

const QList<QString>& BodyInfo::get_head_list(){
    if (_head_list.isEmpty()){
       _head_list.append("Neck");
       _head_list.append("Head");
       _head_list.append("Head_End");
    }
}
