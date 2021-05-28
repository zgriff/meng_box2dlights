//
//  PointLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a point light object. A point light is a light
//  source at a given point with rays extending out in 360 degrees. It can
//  be thought of as a cone light with coneDegreee set to 360.0f.
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

#include "PointLight.h"
#include <cmath>
#include <math.h>

using namespace cugl::b2dlights;

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new point light object with the given parameters.
 *
 * @param  pos  Initial position in world coordinates
 * @param  numRays  Number of rays in the light
 * @param  radius  Radius of the point light circle
 *
 * @return  true if the light is initialized properly, false otherwise.
 */
bool PointLight::init(const Vec2 pos, int numRays, float radius) {
    
    PositionalLight::init(pos, numRays, radius);
        
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
bool PointLight::calculateEndpoints() {
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    
    //Divide circle around light into equal parts
    float angleNum = 360.0f / (_numRays - 1.0f);
    
    for (int i = 0; i < _numRays; i++) {
        
        float angle = angleNum * i * (M_PI / 180) ;
        
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        
        _endX[i] = (_radius * cosarr[i]);
        _endY[i] = (_radius * sinarr[i]);
    }
    
    return true;
}


