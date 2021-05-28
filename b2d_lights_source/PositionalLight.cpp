//
//  PositionalLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a positional light object. It serves to provide a
//  uniform interface for all location based lights. However, it still cannot
//  be instantiated directly, as the correct instantiation depends on the shape
//  of the light. See PointLight and ConeLight for concrete examples.
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

#include "PositionalLight.h"

using namespace cugl::b2dlights;


#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new positional light object with the given parameters.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos  Initial position in world coordinates
 * @param  numRays  Number of rays in the light
 * @param  radius  Radius of the point light circle
 *
 * @return  true if the light is initialized properly, false otherwise.
 */
bool PositionalLight::init(const Vec2 pos, const int numRays, const float radius) {
    
    Light::init(pos, numRays);
    
    _radius = radius;
        
    return true;
}


#pragma mark -
#pragma mark Update

/**
 * Recalculates the light mesh from state and world changes
 *
 * This function needs to be called after altering information in this light object,
 * e.g. color, numRays, etc. It requires a shared pointer to the ObstacleWorld
 * for raycasting. Implementations of this method should NOT retain ownership of the
 * Box2D world. That is a tight coupling that we should avoid.
 *
 * @param  delta  Timing values from parent loop
 * @param  world  The current physics world
 *
 * @return a new point light object at the given point with no radius.
 */
void PositionalLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints();
    
    _dirty = false;
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Mesh Generation

/**
 * Generates the light mesh based on the type of light and world snapshot.
 *
 * This method uses the position of the light and the calculated ray
 * endpoints raycast in the provided world. If a ray hits a fixture, we record
 * the new ray endpoint and the fraction of the magnitude of the original ray
 * over the magnitude of the new ray. Implementations of this method should
 * not retain ownership of the world as that is tight coupling.
 *
 * @param  world  The current ObstacleWorld of the game.
 *
 * @return  true if the vector of LightVerts  was successfully populated, false otherwise.
 */
bool PositionalLight::calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    _lightVerts.clear();
    _lightIndx.clear();
    
    //Check to see that endpoints have been initialized before creating mesh
    if (_dirty) {
        calculateEndpoints();
    }
    
    Vec2 tmpEnd;
    
    for (int i = 0; i < _numRays; i++) {
        m_index = i;
        f[i] = (1.0f);
        tmpEnd.x = _endX[i] + getPosition().x;
        mx[i] = (tmpEnd.x);
        tmpEnd.y = _endY[i] + getPosition().y;
        my[i] = (tmpEnd.y);
        
        world->rayCast(ray, getPosition(), tmpEnd);
    }
    
    //Start with center of light, then iterate through all outside verts
    LightVert light;
    
    light.pos = Vec2(getPosition().x,getPosition().y);
    light.color = _color;
    light.frac = 1.0f;
    
    _lightVerts.push_back(light);
    
    _lightIndx.push_back(0);
            
    for (int i = 0; i < _numRays; i++) {
        
        light.pos = Vec2(mx[i],my[i]);
        light.color = _color;
        light.frac = 1.0f - f[i];
        
        _lightVerts.push_back(light);
        
        _lightIndx.push_back(i+1);
    }
    
    return true;
}


#pragma mark -
#pragma mark Light Mesh Querying

/**
 * Queries this light's mesh for the given point
 *
 * @param  x  The x-coordinate of the point to query
 * @param  y  The y-coordinate of the point to query
 *
 * @return  true if the point lies within the light's mesh
 */
bool PositionalLight::contains(float x, float y) {
    
    bool result = false;
    
    // quick distance check
    float x_d = getPosition().x - x;
    float y_d = getPosition().y - y;
    float dst2 = x_d * x_d + y_d * y_d;
    
    if (_radius * _radius <= dst2) return false;

    // actual check
    float x2 = getPosition().x;
    float y2 = getPosition().y;
    float x1, y1;
    
    for (int i = 0; i <= _numRays; x2 = x1, y2 = y1, i++) {
        if (i == _numRays) {
            x1 = getPosition().x;
            y1 = getPosition().y;
        } else {
            x1 = mx[i];
            y1 = my[i];
        }
        if (((y1 < y) && (y2 >= y)) || ((y1 >= y) && (y2 < y))) {
            if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) result = !result;
        }
    }
    
    return result;
}

