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
#include "animatednode.h"

AnimatedNode::AnimatedNode() {
    _extremity_translation_curves.set_interpolation(Curve::bezier);
    _rotation_curves.set_interpolation(Curve::bezier);
}

const btTransform AnimatedNode::getWorldTransform(const btTransform parent_transform, float time) const{
    btTransform object_transform;
    btQuaternion quat;

    btVector3 xyz = btVector3(deg2rad(rotationVector(time)));
    btQuaternion bx(btVector3(1,0,0),xyz.x()),
                 by(btVector3(0,1,0),xyz.y()),
                 bz(btVector3(0,0,1),xyz.z());
    quat = bz * by * bx; // conversion from euler angles xyz order to yxz
    quat.normalize();

    object_transform.setOrigin(TranslationVector(time));
    object_transform.setRotation(quat);
    return parent_transform * object_transform ;
}


