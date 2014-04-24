#include "utils.h"


////heading
//float rot_x(btQuaternion q)
//{
//    float m_x = q.x();
//    float m_y = q.y();
//    float m_z = q.z();
//    float m_w = q.w();
//    float rotx = atan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
//    return rad2deg(rotx);
//}

////attitude
//float rot_y(btQuaternion q)
//{
//    float m_x = q.x();
//    float m_y = q.y();
//    float m_z = q.z();
//    float m_w = q.w();
//    float roty = asin(2 * ((m_w * m_y) + (m_z * m_x)));
//    return rad2deg(roty);
//}

////bank
//float rot_z(btQuaternion q)
//{
//    float m_x = q.x();
//    float m_y = q.y();
//    float m_z = q.z();
//    float m_w = q.w();
//    float rotz = atan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
//    return rad2deg(rotz);
//}

//heading
float rot_x(float x , float y , float z , float w)
{
    float m_x = x;
    float m_y = y;
    float m_z = z;
    float m_w = w;
    float rotx = atan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
    return rad2deg(rotx);
}

//attitude
float rot_y(float x , float y , float z , float w)
{
    float m_x = x;
    float m_y = y;
    float m_z = z;
    float m_w = w;
    float roty = asin(2 * ((m_w * m_y) - (m_z * m_x)));
    return rad2deg(roty);
}

//bank
float rot_z(float x , float y , float z , float w)
{
    float m_x = x;
    float m_y = y;
    float m_z = z;
    float m_w = w;
    float rotz = atan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
    return rad2deg(rotz);
}


// m_x = qy?
// m_y = qz?
// m_z = qx?
// m_w = qw?

btVector3 btQuat2euler(btQuaternion q){
//    toString(q,"quaternion : ");
//    qDebug()<<"scalar "<<q.
    q.normalize();
//    q=q.inverse();
    float x, y , z , w;
    x = q.z();
    y = q.x();
    z = q.y();
    w = q.w();
    btVector3 vect(rot_y(x,y,z,w),rot_z(x,y,z,w),rot_x(x,y,z,w));
    return btVector3(vect);
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
void toString(const btQuaternion& quaternion,const QString& prefix){
    qDebug()<<prefix<<": "<<quaternion.x()<<" "<<quaternion.y()<<" "<<quaternion.z()<<" "<<quaternion.w()<<" ";
}
