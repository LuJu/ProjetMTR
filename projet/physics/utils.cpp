#include "utils.h"
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

btVector3 btQuat2euler(btQuaternion q){
    q.normalize();
    float x, y , z , w;
    x = q.z();
    y = q.x();
    z = q.y();
    w = q.w();
    btVector3 vect(rot_y(x,y,z,w),rot_z(x,y,z,w),rot_x(x,y,z,w));
    return btVector3(vect);
}

double translationKineticEnergy(double speed, double mass){
    return (speed*speed*mass)/2;
}

double potentialEnergy(double mass, double gravitation, double height){
    return (mass*gravitation*height);
}

btScalar kineticMoment(btVector3 rotation_axis,btVector3 shape, btScalar mass){
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

btScalar angularKineticEnergy(btVector3 angular_velocity, btVector3 rotation_vector_diff, btVector3 shape, btScalar mass ){
    return pow(angular_velocity.length(),2) * kineticMoment(rotation_vector_diff,shape,mass) / 2;
}
