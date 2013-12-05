#ifndef BODYINFO_H
#define BODYINFO_H

#include <QString>
#include <QHash>
class BodyInfo
{
public:
    BodyInfo();
    static float mass(QString body_part, float total_mass);
};

#endif // BODYINFO_H
