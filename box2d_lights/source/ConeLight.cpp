//
//  ConeLight.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "ConeLight.h"
#include <cmath>
#include <math.h>

using namespace cugl::b2dlights;

ConeLight::~ConeLight(void) {
    _sceneNode = nullptr;
}

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new wheel object of the given dimensions.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  radius   The wheel radius
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool ConeLight::init(const Vec2 pos, int numRays, float radius, float direction, float degree) {
    
    PositionalLight::init(pos, numRays, radius);
    
    _direction = direction;
    _coneDegree = degree;
    
    calculateEndpoints();
    
    return true;
}


void ConeLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints();
    
    _dirty = false;
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Mesh Calculations

bool ConeLight::calculateEndpoints() {
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    
    for (int i = 0; i < _numRays; i++) {
        //Divide cone into equal parts
        float angle = (_direction + (0.5f * _coneDegree) - (_coneDegree * i / (_numRays - 1.0f))) * (M_PI / 180);
        
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        
        _endX.push_back(_radius * cosarr[i]);
        _endY.push_back(_radius * sinarr[i]);
    }
    
    return true;
}
    
