//
//  ConeLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a cone light object. A cone light is a positional
//  light with rays casted in a cone shape in a specific direction.
//
//  This class uses our standard shared-pointer architecture.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  This file is based on CUObstacle, CUSimpleObstacle, and CUBoxObstacle
//  by Walker White, 2021
//
//  Author: Zach Griffin
//  Version: 5/28/21

#include "ConeLight.h"
#include <cmath>
#include <math.h>

using namespace cugl::b2dlights;


#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new cone light object with the given parameters.
 *
 * @param  pos  Initial position in world coordinates
 * @param  numRays  Number of rays in the light
 * @param  radius  Radius of the point light circle
 * @param  direction  Radius of the point light circle
 * @param  degree  Radius of the point light circle
 *
 * @return true if the obstacle is initialized properly, false otherwise.
 */
bool ConeLight::init(const Vec2 pos, int numRays, float radius, float direction, float degree) {
    
    PositionalLight::init(pos, numRays, radius);
    
    _direction = direction;
    _coneDegree = degree;
        
    return true;
}


#pragma mark -
#pragma mark Light Mesh Generation

/**
 * Calculates the ray endpoints based on light parameters
 *
 * Any time informatiom such as numRays or radius is changed,
 * the endpoints must be updated. Note: Endpoints are relative to the light's
 * position, and changes in position are calculated in calculateLightMesh.
 *
 * @return true if successfully able to update ray endpoints
 */
bool ConeLight::calculateEndpoints() {
    
    CULog("cone pos: %f , %f ", getPosition().x,getPosition().y);
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    
    for (int i = 0; i < _numRays; i++) {
        //Divide cone into equal parts
        float angle = (_direction + (0.5f * _coneDegree) - (_coneDegree * i / (_numRays - 1.0f))) * (M_PI / 180);
        
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        
        _endX[i] = (_radius * cosarr[i]);
        _endY[i] = (_radius * sinarr[i]);
    }
    
    return true;
}
    
