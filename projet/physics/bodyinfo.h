#ifndef BODYINFO_H
#define BODYINFO_H

#include <QString>
#include <QStringList>
#include <QHash>

class BodyInfo
{
public:
    static float mass(const QString& body_part, float total_mass);
    static QList<QStringList> jointList();
    static QList<QPair<QString,QString> > angularCalculationList();
private:
    virtual void abstract() = 0;
};

#endif // BODYINFO_H
