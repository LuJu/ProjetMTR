#include "utils.h"
//heading
btScalar rot_x(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar rotx = qAtan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
//    qDebug()<<"1: "<<2*((m_w * m_x) + (m_y * m_z));
//    qDebug()<<"2: "<<1 - (2 * ((m_x* m_x) + (m_y * m_y)));
    return rad2deg(rotx);
}

//attitude
btScalar rot_y(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar roty = asin(2 * ((m_w * m_y) - (m_z * m_x)));
    return rad2deg(roty);
}

//bank
btScalar rot_z(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar rotz = qAtan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
    return rad2deg(rotz);
}

// m_x = qy?
// m_y = qz?
// m_z = qx?
// m_w = qw?


btVector3 btQuat2euler(btQuaternion q){
    q.normalize();
    btVector3 vect;
    btScalar x, y , z , w;
    btScalar heading,attitude,bank;
    x = q.z();
    y = q.x();
    z = q.y();
    w = q.w();
    btScalar test = x*z + y*w;
//    qDebug()<<"test "<<test;
//    if (test > .499 ) {
//        heading = 2*atan2(z,w);
//        attitude = M_PI_2;
//        bank = 0;
//    } else if (test < -.499 ) {
//        heading = -2*atan2(z,w);
//        attitude = - M_PI_2;
//        bank = 0;
//    } else {
        heading = rot_x(x,y,z,w);
        attitude = rot_y(x,y,z,w);
        bank = rot_z(x,y,z,w);
//    }
    vect = btVector3(attitude,bank,heading);
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

btVector3 deg2rad(const btVector3& vector){
    return btVector3((vector.x()/180)*M_PI,(vector.y()/180)*M_PI,(vector.z()/180)*M_PI);
}
btVector3 rad2deg(const btVector3& vector){
    return btVector3((vector.x()/M_PI)*180,(vector.y()/M_PI)*180,(vector.z()/M_PI)*180);
}

//btQuaternion derivated(const btQuaternion& quat){
//    btQuaternion qp;
//    qp = 0.5 * quat.w() * quat;
//    return qp;
//}
btVector3 angleNormalize(const btVector3 rot){
    btVector3 r;
//    qDebug()<<M_PI;
//    qDebug()<<rot.z();
    if (rot.x() > M_PI)
        r.setX(rot.x()-M_PI*2.0);
    if (rot.y() > M_PI)
        r.setY(rot.y()-M_PI*2.0);
    if (rot.z() > M_PI)
        r.setZ(rot.z()-M_PI*2.0);
    if (rot.x() < -M_PI)
        r.setX(rot.x()+M_PI*2.0);
    if (rot.y() < -M_PI)
        r.setY(rot.y()+M_PI*2.0);
    if (rot.z() < -M_PI)
        r.setZ(rot.z()+M_PI*2.0);
//    qDebug()<<toString(r,"norm:: ");
    return r;
}
