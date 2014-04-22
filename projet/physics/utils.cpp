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

double kinetic_energy(double speed, double mass){
    return (speed*speed*mass)/2;
}

double potential_energy(double mass, double gravitation, double height){
    return (mass*gravitation*height);
}

btScalar get_moment(btVector3 rotation_axis,btVector3 shape, btScalar mass){
    btScalar m = mass;
    btScalar R = shape.x();
    btScalar R2 = pow(R,2);
    btScalar h = shape.y();
    btScalar h2 = pow(h,2);
    btMatrix3x3 moment_matrix (m*(R2/4.0f + h2/12.0f) , 0               , 0                      ,
                               0                      , (m * R2)/2.0f  , 0                      ,
                               0                      , 0               , m*(R2/4.0f + h2/12.0f) );
    btMatrix3x3 rotation;
    rotation.setIdentity();
    rotation[0][0] = rotation_axis[0];
    rotation[1][0] = rotation_axis[1];
    rotation[2][0] = rotation_axis[2];
    rotation[1][1] = 0;
    rotation[2][2] = 0;
    btMatrix3x3 product = (moment_matrix*rotation);
    return (product.getColumn(0)).length();
}

void toString(const btVector3& vector,const QString& prefix){
    qDebug()<<prefix<<": "<<vector.x()<<" "<<vector.y()<<" "<<vector.z()<<" ";
}
