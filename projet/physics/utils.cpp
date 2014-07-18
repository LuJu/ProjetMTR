/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "utils.h"
//heading
btScalar rot_x(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar rotx = qAtan2(2*((m_w * m_x) + (m_y * m_z)), 1 - (2 * ((m_x* m_x) + (m_y * m_y))));
    return rotx;
}

//attitude
btScalar rot_y(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar roty = asin(2 * ((m_w * m_y) - (m_z * m_x)));
    return roty;
}

//bank
btScalar rot_z(btScalar x , btScalar y , btScalar z , btScalar w)
{
    btScalar m_x = x;
    btScalar m_y = y;
    btScalar m_z = z;
    btScalar m_w = w;
    btScalar rotz = qAtan2(2 * ((m_w * m_z) + (m_x * m_y)), 1 - (2 * ((m_y * m_y) + (m_z * m_z))));
    return rotz;
}

btVector3 btQuat2Euler(btQuaternion q){
    q.normalize();
    btVector3 vect;
    btScalar x, y , z , w;
    btScalar heading,attitude,bank;
    x = q.z();
    y = q.x();
    z = q.y();
    w = q.w();
    btScalar test = x*z + y*w;
    if (test > .499 ) {
        heading = 2*atan2(z,w);
        attitude = M_PI_2;
        bank = 0;
    } else if (test < -.499 ) {
        heading = -2*atan2(z,w);
        attitude = - M_PI_2;
        bank = 0;
    } else {
        heading = rot_x(x,y,z,w);
        attitude = rot_y(x,y,z,w);
        bank = rot_z(x,y,z,w);
    }
    vect = btVector3(attitude,bank,heading);
    return btVector3(vect);
}

double translationKineticEnergy(double speed, double mass){
    return (speed*speed*mass)/2;
}

double potentialEnergy(double mass, double gravitation, double height){
    return (mass*gravitation*height);
}

float kineticMoment(btVector3 rotation_axis,btVector3 shape, btScalar mass){
    float m = mass;
    float R = shape.x();
    float R2 = pow(R,2);
    float h = shape.y();
    float h2 = pow(h,2);
    btMatrix3x3 moment_matrix (m*(R2/4.0f + h2/12.0f) , 0               , 0                      ,
                               0                      , (m * R2)/2.0f   , 0                      ,
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

float angularKineticEnergy(btVector3 angular_velocity, btVector3 rotation_vector_diff, btVector3 shape, btScalar mass ){
    return pow(angular_velocity.length(),2) * kineticMoment(rotation_vector_diff,shape,mass) / 2;
}

btVector3 deg2rad(const btVector3& vector){
    return btVector3((vector.x()/180)*M_PI,(vector.y()/180)*M_PI,(vector.z()/180)*M_PI);
}
btVector3 rad2deg(const btVector3& vector){
    return btVector3((vector.x()/M_PI)*180,(vector.y()/M_PI)*180,(vector.z()/M_PI)*180);
}

//! Return the vector rotated by the quaternion
/*!
    \param  quat : The quaternion representing the rotation given
    \param  quat : The vector to be rotated
    \return : The vector rotateduu
*/
btVector3 rotatedVector(btQuaternion quat, btVector3 vector){
    quat.normalize();
    QQuaternion qquat(quat.w(),quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z());
    qquat= qquat.fromAxisAndAngle(quat.getAxis().x(),quat.getAxis().y(),quat.getAxis().z(),rad2deg(quat.getAngle()));

    QVector3D qvect(vector.x(),vector.y(),vector.z());
    qvect = qquat.rotatedVector(qvect);
    return btVector3(qvect.x(),qvect.y(),qvect.z());
}

btQuaternion differentiate(btQuaternion quat){
    
}

btQuaternion exp(btQuaternion quat){
    btScalar angle = quat.w();
    btQuaternion ret;
//    ret.setW();
}

btScalar angleDifference(btVector3 v1, btVector3 v2){
    btScalar dot_product = v1.dot(v2);
    btScalar cost = dot_product / (v2.length() * v1.length());
    btScalar rotation_angle = qAcos(cost);
    return rotation_angle;
}
