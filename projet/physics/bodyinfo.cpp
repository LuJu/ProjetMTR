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
QHash<QString,float> BodyInfo::_sub_segments;
QHash<QString,float> BodyInfo::_segments;
QHash<QString,QString> BodyInfo::_segments_parenting;
CSVParser BodyInfo::_joint_values;

float BodyInfo::subSegmentMass(const QString& body_part, float total_mass){
    float proportion;
    if (_sub_segments.size() == 0){
        CSVParser parser;
        QString filepath("../assets/CSV/input/segments_subs.csv");
        QString sub_segment_name;
        float mass;

        if (_sub_segments.size() == 0){
            parser.parseFile(filepath,",");
            if (parser.size() > 0){
                for (int i = 0; i < parser.size(); ++i) {
                    sub_segment_name = parser.at(i).at(0).toLower().replace(" ","");
                    mass =     parser.at(i).at(5).toFloat();
                    _sub_segments.insert(sub_segment_name,mass);
                }
            } else {
                qWarning()<<"Target file "<<filepath<<" not found";
            }
        }
    }
    QHash<QString,float>::const_iterator it = _sub_segments.find(body_part.toLower().replace(" ",""));
    if (it !=_sub_segments.end())
        proportion = it.value();
    else {
        proportion = 0;
    }
    return (total_mass/100) * proportion;
}

const CSVParser& BodyInfo::jointList(){
    static bool first_time = true;
    if (first_time){
        _joint_values.parseFile(":/CSV/input/jointvalues.csv",";");
        first_time = false;
    }
    return _joint_values;
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

void BodyInfo::genFixedInfo(const QString& filepath, QString target_name){
    CSVParser parser;
    QString part_name;
    bool fixed;
    parser.parseFile("../assets/CSV/input/"+filepath,",");
    int index = -1;
    if (parser.size() > 0){
        const QStringList& names = parser.at(0);
        for (int i = 1; i < names.size(); ++i) {
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
    if (fixed != _segments_fixed.end()) { return fixed.value(); }
    else qWarning()<<"Part "<<part_name<<" not found";
}
