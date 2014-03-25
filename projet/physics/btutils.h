#ifndef BTUTILS_H
#define BTUTILS_H

#include "btBulletCollisionCommon.h"
#include "utils/maths.h"

float rot_x(btQuaternion q);
float rot_y(btQuaternion q);
float rot_z(btQuaternion q);

btVector3 btQuat2euler(btQuaternion q);


#endif // BTUTILS_H
