//
//  PointLight.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "PointLight.h"
#include <cmath>
#include <math.h>

using namespace cugl::b2dlights;


PointLight::~PointLight(void) {
    _sceneNode = nullptr;
}


#pragma mark -
#pragma mark Constructors


bool PointLight::init(const Vec2 pos, int numRays, float radius) {
    
    PositionalLight::init(pos, numRays, radius);
    
    calculateEndpoints();
    
    return true;
}


void PointLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints();
    
    _dirty = false;
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Querying


bool PointLight::calculateEndpoints() {
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    
    //Divide circle around light into equal parts
    float angleNum = 360.0f / (_numRays - 1.0f);
    
    for (int i = 0; i < _numRays; i++) {
        
        float angle = angleNum * i * (M_PI / 180) ;
        
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        
        _endX.push_back(_radius * cosarr[i]);
        _endY.push_back(_radius * sinarr[i]);
    }
    
    return true;
}


