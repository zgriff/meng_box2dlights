//
//  PointLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "Light.h"

using namespace cugl;

class PointLight : public Light {
protected:
    
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    
    Color4 _defaultColor = Color4(0.75f, 0.75f, 0.5f, 0.75f);
    int _minRays = 3;
    
    Color4 _color;
    
    Vec2 _pos;
    
    
    bool _staticLight = false;
    
    bool _culled = false;
    
    bool _dirty = true;
    
    bool _ignoreBody = false;
    
    int _numRays;
    int _numVerts;
    
    float _distance;
    float _direction;
    float _colorF;
    float _softShadowLength = 2.5f;
    
    
    Mesh<SpriteVertex2> lightMesh;
    Mesh<SpriteVertex2> softShadowMesh;
    
    
//    float segments[];
//    float mx[];
//    float my[];
//    float f[];
    int m_index = 0;
    
    
    
public:
#pragma mark -
#pragma mark Getters and Setters
    
    void setColor(Color4 color) {_color = color;}
    
    void setColor(float r, float g, float b, float a) {
        _color = Color4(r,g,b,a);
    }
    
    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {return _sceneNode;}
    
    
    
    
    
};


#endif /* PointLight_h */
