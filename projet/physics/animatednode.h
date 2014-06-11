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
#ifndef ANIMATEDNODE_H
#define ANIMATEDNODE_H

#include "btBulletCollisionCommon.h"

#include "utils/triplet.h"
#include "utils/curve.h"

#include "utils.h"


//! An animated 3D node
/*!
        Represents an animated 3D object. The values of the movement at each time is contained in the three 3DCurves
        This does not represent a limb of the body. The limbs of the body are represented by the Part class
    */
class AnimatedNode
{
public:
    AnimatedNode();

    btVector3 scalingVector(float time) const {
        return btVector3(_scaling_curves[0].get_value(time),
                         _scaling_curves[1].get_value(time),
                         _scaling_curves[2].get_value(time));
    }
    btVector3 TranslationVector(float time) const {
        return btVector3(_extremity_translation_curves[0].get_value(time),
                         _extremity_translation_curves[1].get_value(time),
                         _extremity_translation_curves[2].get_value(time));
    }
    btVector3 rotationVector(float time) const {
        return btVector3(_rotation_curves[0].get_value(time),
                         _rotation_curves[1].get_value(time),
                         _rotation_curves[2].get_value(time));
    }
    Curve3d& get_scaling_curves() {return _scaling_curves;}
    Curve3d& get_rotation_curves() {return _rotation_curves;}
    Curve3d& get_translation_curves() {return _extremity_translation_curves;}

    const btTransform getWorldTransform(const btTransform parent_transform, float time) const;
    state_information _information;

private:
    Curve3d _scaling_curves;
    Curve3d _rotation_curves;
    Curve3d _extremity_translation_curves;

};

#endif // ANIMATEDNODE_H
