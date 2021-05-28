//
//  PositionalLight.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "PositionalLight.h"

using namespace cugl::b2dlights;


#pragma mark -
#pragma mark Constructors

bool PositionalLight::init(const Vec2 pos, const int numRays, const float radius) {
    
    Light::init(pos, numRays);
    
    _radius = radius;
        
    return true;
}


#pragma mark -
#pragma mark Update

void PositionalLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints();
    
    _dirty = false;
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Mesh Generation

bool PositionalLight::calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
//    mx.clear();
//    my.clear();
//    f.clear();
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

