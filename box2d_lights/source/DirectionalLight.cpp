//
//  DirectionalLight.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "DirectionalLight.h"

using namespace cugl::b2dlights;

#pragma mark -
#pragma mark Constructors

bool DirectionalLight::init(const Vec2 pos, const int numRays, const float direction, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    Light::init(pos, numRays);
    
    _direction = direction;
    
    _startX = new float[numRays];
    _startY = new float[numRays];
    
    calculateEndpoints(world);
    
    return true;
}


#pragma mark -
#pragma mark Update

void DirectionalLight::update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    if (_dirty) calculateEndpoints(world);
    
    calculateLightMesh(world);
}


#pragma mark -
#pragma mark Light Mesh Generation

bool DirectionalLight::calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
    
    mx.clear();
    my.clear();
    f.clear();
    _lightVerts.clear();
    _lightIndx.clear();

    for (int i = 0; i < _numRays; i++) {
        
        m_index = i;
        f.push_back(1.0f);
        mx.push_back(_endX[i]);
        my.push_back(_endY[i]);
        
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
        
        _endX.push_back(stepX + xAxelOffset);
        _endY.push_back(stepY + yAxelOffset);
    }
    
    return true;
}


#pragma mark -
#pragma mark Light Mesh Querying
    
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
