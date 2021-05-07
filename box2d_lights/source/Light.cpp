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
#include "Globals.h"


#define MIN_RAYS        50




using namespace cugl;

Light::~Light(void) {
    _lightMesh.clear();
    _softShadowMesh.clear();
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
    
    return true;
}


void Light::setTextures(const std::shared_ptr<Texture>& lightText) {

    _sceneNode = scene2::PolygonNode::allocWithTexture(lightText, _poly*_drawscale);
    _sceneNode->setPosition(getPosition()*_drawscale);
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
//    _texture = lightText;
    _body->SetUserData(this);
}


void Light::setDrawScale(float scale) {
    _drawscale = scale;
    if (_sceneNode != nullptr) {
        _sceneNode->setPosition(getPosition()*globals::BOX2D_TO_SCENE);
    }
}

void Light::update(float delta) {
    Obstacle::update(delta);
    if (clock() - _updateTimer  >= _updateRate) {
        calculateLightMesh();
        _updateTimer = clock();
    }
    
    if (_sceneNode != nullptr) {
        _sceneNode->setPosition(getPosition()*globals::BOX2D_TO_SCENE);
    }
}


Mesh<SpriteVertex2> Light::calculateLightMesh() {
    Mesh<SpriteVertex2> result;
    
    mx.clear();
    my.clear();
    f.clear();
    segments.clear();
    _triangulator.clear();
    
    
    
    Vec2 start;
    Vec2 vec = getPosition();
//    auto angle = getAngle();
//    float cosine = cos(angle);
//    float sine = sin(angle);
//    float dX = vec.x * cosine - vec.y * sine;
//    float dY = vec.x * sine + vec.y * cosine;
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
    
//    std::vector<float> segments;
//
//    float segments[_numRays * 4];
    
        
//    segments.push_back(start.x);
//    segments.push_back(start.y);
//    segments.push_back(_color.getRGBA());
//    segments.push_back(1.0f);
    
    //-1 bc last is repeat of first
    for (int i = 0; i < _numRays-1; i++) {
        
        segments.push_back(mx[i]);
        segments.push_back(my[i]);
//        segments.push_back(_color.getRGBA());
//        segments.push_back(1.0f - f[i]);
    }
    
    
    _triangulator.set(segments);
//    tri.addSteiner(start);
    _triangulator.calculate();
    
    _poly = _triangulator.getPolygon();
    _lightMesh = _triangulator.getPolygon();
    
    if (_sceneNode != nullptr) {
        _sceneNode->setPolygon(_triangulator.getPolygon()*_drawscale);
    }
    
//    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
//    _body->SetUserData(this);
    
    return _lightMesh;
}
