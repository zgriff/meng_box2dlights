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
#include "Globals.h"


#define MIN_RAYS        50


using namespace cugl;

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
bool ConeLight::init(const Vec2 pos, int numRays, float radius, float direction, float size) {
//    WheelObstacle::init(pos, 1.0f);
//    setSensor(true);
    Light::init(pos, numRays);
    _direction = direction;
    _size = size;
    _color = _defaultColor;
    _numRays = numRays;
    _radius = radius;
    _updateTimer = clock();
    
    return true;
}


void ConeLight::calculateLightMesh() {
    mx.clear();
    my.clear();
    f.clear();
    _lightVerts.clear();
    _lightIndx.clear();
    
    Vec2 start = getPosition();

    Vec2 tmpEnd;
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    float endX[_numRays];
    float endY[_numRays];
    
    //Divide circle around light into equal parts
    float angleNum = 360.0f / (_numRays - 1.0f);
    for (int i = 0; i < _numRays; i++) {
        float angle = angleNum * i * (M_PI / 180) ;
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        endX[i] = _radius * cosarr[i];
        endY[i] = _radius * sinarr[i];
    }
    
    for (int i = 0; i < _numRays; i++) {
        m_index = i;
        f.push_back(1.0f);
        tmpEnd.x = endX[i] + start.x;
        mx.push_back(tmpEnd.x);
        tmpEnd.y = endY[i] + start.y;
        my.push_back(tmpEnd.y);
        if (_world != nullptr) {
            _world->rayCast(ray, start, tmpEnd);
        }
    }
    
    //Start with center of light, then iterate through all outside verts
    LightVert light;
    
    light.pos = Vec2(start.x,start.y);
    light.color = _color;
    light.frac = 1.0f;
    
    _lightVerts.push_back(light);
            
    for (int i = 0; i < _numRays; i++) {
        
        light.pos = Vec2(mx[i],my[i]);
        light.color = _color;
        light.frac = 1.0f - f[i];
        
        _lightVerts.push_back(light);
        
        _lightIndx.push_back(i+1);
        
        //if last, _lightIndxex to first vert on outside of poly (0 _lightIndxex is center)
        if (i == _numRays-1) {
            _lightIndx.push_back(1);
        } else {
            _lightIndx.push_back(i+2);
        }
        
        _lightIndx.push_back(0);
    }
    
}
