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
#ifndef BODYINFO_H
#define BODYINFO_H

#include <QString>
#include <QStringList>
#include <QHash>

#include "utils/csvparser.h"

//! Static class that is used to gather the standard information on a human body (mass of each part etc)
class BodyInfo
{
public:
    //! Returns the mass of the body part in function of the total mass of the body
    /*!
        \param  body_part: the name of the body part
        \param  total_mass: mass of the body (kg)
        \return mass of the body part (kg)
    */
    static float subSegmentMass(const QString& body_part, float total_mass);
    //! Returns the list of the joint types for each limbs
    static const CSVParser& jointList();
    //! Returns whether the part has to be fixed (mass = 0) or not
    /*!
      Returns whether the part has to be fixed (mass = 0) or not. \n
      in order to work, the genFixedInfo() method must have been called once
        \param  part_name: the name of the body part
    */
    static bool isFixed(QString part_name);
    //! Returns the parent segment of the subsegment given in parameter
    static QString getSegment(const QString& sub_segment);
    static QStringList getSegmentList(){return _segments;}

    static void loadData(QString file_name);
private:
    virtual void abstract() = 0;
    //! generates the list of fixed limbs
    /*!
        \param  filepath : the path of the csv file containing limbs list
        \param  target_name : the name of the animation
    */
    static void genFixedInfo(QString target_name);
    static void loadSegmentsData();
    static QHash<QString,bool> _segments_fixed;
    static QHash<QString,float> _sub_segments;
    static QStringList _segments;
    static QHash<QString,QString> _segments_parenting;
    static CSVParser _joint_values;
};

#endif // BODYINFO_H
