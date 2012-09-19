﻿
// Copyright (c) 2011-2012, Daniel Müller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the 
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#pragma once
#ifndef __MOON2_H__
#define __MOON2_H__

#include "declspec.h"
#include "typedefs.h"
#include "julianday.h"
#include "coords.h"


namespace osgHimmel
{

class OSGH_API Moon2 // Lower Accuracy
{
public:

    static const float meanLongitude(const t_julianDay t); 
    static const float meanElongation(const t_julianDay t);
    static const float meanAnomaly(const t_julianDay t);
    static const float meanLatitude(const t_julianDay t);

    static const float meanOrbitLongitude(const t_julianDay t);

    static const t_eclf position(const t_julianDay t);
    static const t_equf apparentPosition(const t_julianDay t);

    static const t_horf horizontalPosition(
        const t_aTime &aTime
    ,   const float latitude
    ,   const float longitude);

    static const float distance(const t_julianDay t);

    static void opticalLibrations(
        const t_julianDay t
    ,   float &l /* librations in longitude */
    ,   float &b /* librations in latitude  */);

    static const float parallacticAngle(
        const t_aTime &aTime
    ,   const float latitude
    ,   const float longitude);

    static const float positionAngleOfAxis(const t_julianDay t);

    static const float meanRadius();
};

} // namespace osgHimmel

#endif // __MOON2_H__