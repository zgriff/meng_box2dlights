//
//  DirectionalLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a directional light object. This type of light will
//  cover the entire ObstacleWorld it is passed, with the rays pointing in a
//  given angle.
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

#include "DirectionalLight.h"

using namespace cugl::b2dlights;


#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new directional light object with the provided parameters.
 *
 * @param  pos  Initial position in world coordinates
 * @param  numRays  Number of rays in the light
 * @param  radius  Radius of the point light circle
 *
 * @return true if the obstacle is initialized properly, false otherwise.
 */
bool DirectionalLight::init(const int numRays, const float direction) {
    
    Light::init(Vec2::ZERO, numRays);
    
    _direction = direction;
    
    _startX = new float[numRays];
    _startY = new float[numRays];
    
    return true;
}


#pragma mark -
#pragma mark Update

/**
 * Recalculates the light mesh from state and world changes
 *
 * This function needs to be called after altering information in this light object,
 * e.g. color, numRays, etc. It requires a shared pointer to the ObstacleWorld
 * for raycasting. Implementations of this method should NOT retain ownership
 * of the world. That is a tight coupling that we should avoid.
 *
 * @param  delta  Initial position in world coordinates
 * @param  world  Number of rays in the light
 */
void DirectionalLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints(world);
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Mesh Generation

/**
 * Generates the light mesh based on the type of light and world snapshot.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  world  The current ObstacleWorld of the game.
 *
 * @return  true if the vector of LightVerts  was successfully populated, false otherwise.
 */
bool DirectionalLight::calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    _lightVerts.clear();
    _lightIndx.clear();
    
    if (_dirty) {
        calculateEndpoints(world);
    }

    for (int i = 0; i < _numRays; i++) {
        
        m_index = i;
        f[i] = (1.0f);
        mx[i] = (_endX[i]);
        my[i] = (_endY[i]);
        
        world->rayCast(ray, Vec2(_startX[i],_startY[i]), Vec2(_endX[i],_endY[i]));
        
    }
    
    LightVert light;
            
    for (int i = 0; i < _numRays; i++) {
        
        light.pos = Vec2(_startX[i],_startY[i]);
        light.color = _color;
        light.frac = 1.0f;
        
        _lightVerts.push_back(light);
        
        _lightIndx.push_back(2*i);
        
        light.pos = Vec2(mx[i],my[i]);
        light.color = _color;
        light.frac = 1.0f;
        
        _lightVerts.push_back(light);
        
        _lightIndx.push_back(2*i+1);
    }
    
    return true;
}

/**
 * Calculates the ray endpoints based on light parameters
 *
 * Any time informatiom such as numRays or direction is changed,
 * the endpoints must be updated.
 *
 * @return true if successfully able to update ray endpoints
 */
bool DirectionalLight::calculateEndpoints(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    float width = world->getBounds().size.width;
    float height = world->getBounds().size.height;
    float screenSize = width > height ? width : height;
    
    float angle =  _direction * (M_PI / 180.0f);
    
    float xAxelOffset = screenSize * cos(angle);
    float yAxelOffset = screenSize * sin(angle);
    
    float widthOffset = screenSize * -sin(angle);
    float heightOffset = screenSize * cos(angle);
    
    float x = 0.5f * (world->getBounds().origin.x + world->getBounds().size.width) - widthOffset;
    float y = 0.5f * (world->getBounds().origin.y + world->getBounds().size.width) - heightOffset;
    
    float portionX = 2.0f * widthOffset / (_numRays - 1);
    x = floor(x/(portionX * 2)) * portionX * 2;
    float portionY = 2.0f * heightOffset / (_numRays - 1);
    y = ceil(y/(portionY * 2)) * portionY * 2;
    
    for (int i = 0; i < _numRays; i++) {
        
        float stepX = i * portionX + x;
        float stepY = i * portionY + y;
        
        _startX[i] = stepX - xAxelOffset;
        _startY[i] = stepY - yAxelOffset;
        
        _endX[i] = (stepX + xAxelOffset);
        _endY[i] = (stepY + yAxelOffset);
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
bool DirectionalLight::contains(float x, float y) {
    
    bool result = false;

    float x2 = _startX[0];
    float y2 = _startY[0];
    float x1, y1;
    
    for (int i = 0; i <= _numRays; x2 = x1, y2 = y1, ++i) {
        
        if (i == _numRays) {
            x1 = _startX[0];
            y1 = _startY[0];
        } else {
            x1 = mx[i];
            y1 = my[i];
        }
        if (((y1 < y) && (y2 >= y)) || ((y1 >= y) && (y2 < y))) {
            if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) result = !result;
        }
    }
    
    for (int i = 0; i < _numRays; x2 = x1, y2 = y1, ++i) {
        
        x1 = _startX[i];
        y1 = _startY[i];
        if (((y1 < y) && (y2 >= y)) || ((y1 >= y) && (y2 < y))) {
            if ((y - y1) / (y2 - y1) * (x2 - x1) < (x - x1)) result = !result;
        }
    }
    
    return result;
}
