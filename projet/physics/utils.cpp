#include "utils.h"

float rot_x(btQuaternion q)
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float rotx = atan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
    return rad2deg(rotx);
}

float rot_y(btQuaternion q)
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float roty = asin(2 * ((m_w * m_y) - (m_z * m_x)));
    return rad2deg(roty);
}


float rot_z(btQuaternion q)
{
    float m_x = q.x();
    float m_y = q.y();
    float m_z = q.z();
    float m_w = q.w();
    float rotz = atan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
    return rad2deg(rotz);
}

btVector3 btQuat2euler(btQuaternion q){
    return btVector3(rot_x(q),rot_y(q),rot_z(q));
}


