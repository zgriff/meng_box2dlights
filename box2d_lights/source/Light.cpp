//
//  Light.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "Light.h"
#include <cmath>
#include <math.h>


#define MIN_RAYS        50


using namespace cugl;

Light::~Light(void) {
    _poly.clear();
    _lightMesh.clear();
    _softShadowMesh.clear();
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
bool Light::init(const Vec2 pos, float radius) {
    WheelObstacle::init(pos, radius);
    setSensor(true);
    _numRays = MIN_RAYS;
    _distance = 10.0f;
//    _sceneNode = scene2::PolygonNode::alloc();
    _updateTimer = clock();
    _blendEquation = GL_FUNC_ADD;
    _srcFactor = GL_SRC_ALPHA;
    _dstFactor = GL_ONE_MINUS_SRC_ALPHA;
    
    return true;
}



void Light::setDrawScale(float scale) {
    _drawscale = scale;
    _sceneNode = scene2::PolygonNode::alloc(_poly*_drawscale);
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode->setColor(Color4(255.0f,255.0f,255.0f,100.0f));

    _body->SetUserData(this);
    if (_sceneNode != nullptr) {
        _sceneNode->setPosition(getPosition()*_drawscale);
    }
}

void Light::update(float delta) {
    Obstacle::update(delta);
    if (clock() - _updateTimer  >= _updateRate) {
        calculateLightMesh();
        _updateTimer = clock();
    }
    
    if (_sceneNode != nullptr) {
        _sceneNode->setPosition(getPosition()*_drawscale);
    }
}

Mesh<SpriteVertex2> Light::calculateLightMesh() {
    Mesh<SpriteVertex2> result;
    
    mx.clear();
    my.clear();
    f.clear();
    _segments.clear();
    _segmentsMesh.clear();
    
    
    
    Vec2 start;
    Vec2 vec = getPosition();

    start.x = vec.x;
    start.y = vec.y;
//    setDirection(vec + angle * (180.0 / M_PI););
    Vec2 tmpEnd;
    
    float sinarr[_numRays];
    float cosarr[_numRays];
    float endX[_numRays];
    float endY[_numRays];
    
    float angleNum = 360.0f / (_numRays - 1.0f);
    for (int i = 0; i < _numRays; i++) {
        float angle = angleNum * i * (M_PI / 180) ;
        sinarr[i] = sin(angle);
        cosarr[i] = cos(angle);
        endX[i] = _distance * cosarr[i];
        endY[i] = _distance * sinarr[i];
        CULog("endx : %f", endX[i]);
        CULog("endy : %f", endY[i]);
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
    
        
    _segments.push_back(start.x);
    _segments.push_back(start.y);
//    _segments.push_back(_color.getRGBA());
//    _segments.push_back(1.0f);
    std::vector<Uint32> ind;
    
    for (int i = 0; i < _numRays; i++) {
        
        _segments.push_back(mx[i]);
        _segments.push_back(my[i]);
//        _segmentsMesh.push_back(mx[i]);
//        _segmentsMesh.push_back(my[i]);
        ind.push_back(i+1);
        
        //if last, index to first vert on outside of poly (0 index is center)
        if (i == _numRays-1) {
            ind.push_back(1);
        } else {
            ind.push_back(i+2);
        }
        
        ind.push_back(0);
//        _segmentsMesh.push_back(_color.getRGBA());
//        _segmentsMesh.push_back(1.0f - f[i]);
    }
    
    

    
    _poly = Poly2(_segments,ind);
    _lightMesh = Poly2(_segments,ind);
    
    for (size_t ii = 0; ii < _lightMesh.vertices.size(); ii++) {
        const Vec2 pos = _lightMesh.vertices[ii].position;
        _lightMesh.vertices[ii].color = _color;
//        _lightMesh.vertices[ii].texcoord = 1.0f - f[ii];
        
//        float s = pos.x/tsize.width;
//        float t = pos.y/tsize.height;
        
    }
    
    if (_sceneNode != nullptr) {
        _sceneNode->setPolygon(_poly*_drawscale);
    }
    
    
    return _lightMesh;
}
