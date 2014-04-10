#ifndef BODYINFO_H
#define BODYINFO_H

#include <QString>
#include <QStringList>
#include <QHash>

#include "utils/csvparser.h"
class BodyInfo
{
public:
    //! Returns the mass of the body part in function of the total mass of the body
    /*!
        \param  body_part: the name of the body part
        \param  total_mass: mass of the body (kg)
        \return mass of the body part (kg)
    */
    static float mass(const QString& body_part, float total_mass);
    static const CSVParser& jointList();
//    static QList<QStringList> jointList();
//    static QList<QPair<QString,QString> > angularCalculationList();
private:
    virtual void abstract() = 0;
};

#endif // BODYINFO_H
