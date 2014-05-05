#ifndef DEBUGGINGUTILS_H
#define DEBUGGINGUTILS_H

#include <QDebug>
#include <QString>
#include "btBulletCollisionCommon.h"

QString toString(const btVector3& vector, const QString& prefix="");
QString toString(const btQuaternion& quaternion,const QString& prefix="");

#endif // DEBUGGINGUTILS_H
