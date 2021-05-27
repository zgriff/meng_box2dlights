//
//  PositionalLight.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "PositionalLight.h"

using namespace cugl::b2dlights;


bool PositionalLight::init(const Vec2 pos, const int numRays, const float radius) {
    
    Light::init(pos, numRays);
    
    _radius = radius;
    
    return true;
}


//void PositionalLight::update(float delta) {
//
//
//}


bool PositionalLight::contains(float x, float y) {
    
    bool result = false;
    
    // quick distance check
    float x_d = _pos.x - x;
    float y_d = _pos.y - y;
    float dst2 = x_d * x_d + y_d * y_d;
    
    if (_radius * _radius <= dst2) return false;

    // actual check
    float x2 = _pos.x;
    float y2 = _pos.y;
    float x1, y1;
    
    for (int i = 0; i <= _numRays; x2 = x1, y2 = y1, i++) {
        if (i == _numRays) {
            x1 = _pos.x;
            y1 = _pos.y;
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


bool PositionalLight::calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    mx.clear();
    my.clear();
    f.clear();
    _lightVerts.clear();
    _lightIndx.clear();
    
    Vec2 tmpEnd;
    
    for (int i = 0; i < _numRays; i++) {
        m_index = i;
        f.push_back(1.0f);
        tmpEnd.x = _endX[i] + _pos.x;
        mx.push_back(tmpEnd.x);
        tmpEnd.y = _endY[i] + _pos.y;
        my.push_back(tmpEnd.y);
        
        world->rayCast(ray, _pos, tmpEnd);
    }
    
    //Start with center of light, then iterate through all outside verts
    LightVert light;
    
    light.pos = Vec2(_pos.x,_pos.y);
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
