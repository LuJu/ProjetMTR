#include "debuggingutils.h"


QString toString(const btVector3& vector,const QString& prefix){
    return prefix+QString::number(vector.x())+" "+QString::number(vector.y())+" "+QString::number(vector.z())+" ";
}
QString toString(const btQuaternion& quaternion,const QString& prefix){
    return prefix+QString::number(quaternion.x())+" "+QString::number(quaternion.y())+" "+QString::number(quaternion.z())+" "+QString::number(quaternion.w());
}
