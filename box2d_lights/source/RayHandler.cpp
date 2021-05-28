//
//  RayHandler.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/4/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "RayHandler.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include <cugl/render/CUVertexBuffer.h>
#include <cugl/render/CUShader.h>

#include <cugl/cugl.h>

const std::string lightVertShader =
#include "shaders/lightShader.vert"
;

const std::string lightFragShader =
#include "shaders/lightShader.frag"
;


using namespace cugl::b2dlights;

RayHandler::~RayHandler(void) {
    dispose();
}


void RayHandler::dispose() {
    _world = nullptr;
    _vbo = nullptr;
    _shader = nullptr;
}



bool RayHandler::init() {
    SceneNode::initWithPosition(Vec2::ZERO);
    
    _vertSize = 0;
    _indxSize = 0;
    
    _maxVertices = DEFAULT_CAPACITY;
    _maxIndices = 3 * DEFAULT_CAPACITY;
    
    _vertData = new LightVert[_maxVertices];
    _indxData = new GLuint[_maxIndices];
    
    _vbo = VertexBuffer::alloc(sizeof(LightVert));
    _shader = Shader::alloc(SHADER(lightVertShader), SHADER(lightFragShader));
    
    _vbo->setupAttribute("aPosition", 2, GL_FLOAT, GL_FALSE, offsetof(LightVert,pos));
    _vbo->setupAttribute("aColor", 4, GL_FLOAT, GL_FALSE, offsetof(LightVert,color));
    _vbo->setupAttribute("aFrac", 1, GL_FLOAT, GL_FALSE, offsetof(LightVert,frac));
    
    _vbo->attach(_shader);
    
    return true;
}


//Adds a point light to list of lights
//TODO: change so vert data add is in separate function to be updated each frame
bool RayHandler::addPointLight(Vec2 vec, int numRays, float radius) {
    auto light = PointLight::alloc(vec, numRays, radius);
//    light->setWorld(_world);
    light->calculateLightMesh(_world);
    
    CULog("point light contains: %i", light->contains(vec.x+1, vec.y+1));
    
    auto verts = light->getVerts();
    auto indx = light->getIndices();
    
    for (int i = 0; i < verts.size(); i++) {
        _vertData[_vertSize].pos = verts[i].pos*_scale;
        _vertData[_vertSize].color = verts[i].color;
        _vertData[_vertSize].frac = verts[i].frac;
        _vertSize++;
    }
    
    for (int j = 0; j < indx.size(); j++) {
        _indxData[_indxSize] = indx[j];
        _indxSize++;
    }
        
    _lights.push_back(light);
    
    return true;
}


// a point light to list of lights
//TODO: change so vert data add is in separate function to be updated each frame
bool RayHandler::addConeLight(Vec2 vec, int numRays, float radius, float direction, float size) {
    auto light = ConeLight::alloc(vec, numRays, radius, direction, size);
//    light->setWorld(_world);
    light->calculateLightMesh(_world);
    CULog("cone contains: %f, %f : %i", vec.x-1,vec.y-1,light->contains(vec.x-1, vec.y-1));
    
    auto verts = light->getVerts();
    auto indx = light->getIndices();
    
    for (int i = 0; i < verts.size(); i++) {
        _vertData[_vertSize].pos = verts[i].pos*_scale;
        _vertData[_vertSize].color = verts[i].color;
        _vertData[_vertSize].frac = verts[i].frac;
        _vertSize++;
    }
    
    for (int j = 0; j < indx.size(); j++) {
        _indxData[_indxSize] = indx[j];
        _indxSize++;
    }
        
    _lights.push_back(light);
    
    return true;
}

// a directional light to list of lights
//TODO: change so vert data add is in separate function to be updated each frame
bool RayHandler::addDirectionalLight(Vec2 vec, int numRays, float direction) {
    auto light = DirectionalLight::alloc(numRays, direction);
//    light->setWorld(_world);
    light->calculateLightMesh(_world);
    
    CULog("directional light contains: %f, %f : %i", 0.1f,0.0f,light->contains(0.1f, 0.0f));
    CULog("directional light contains: %f, %f : %i", -5.0f, 5.0f,light->contains(5.0f, 5.0f));
    
    auto verts = light->getVerts();
    auto indx = light->getIndices();
    
    for (int i = 0; i < verts.size(); i++) {
        _vertData[_vertSize].pos = verts[i].pos*_scale;
        _vertData[_vertSize].color = verts[i].color;
        _vertData[_vertSize].frac = verts[i].frac;
        _vertSize++;
    }
    
    for (int j = 0; j < indx.size(); j++) {
        _indxData[_indxSize] = indx[j];
        _indxSize++;
    }
        
    _lights.push_back(light);
    
    return true;
}


void RayHandler::update(float delta) {
    
    for (auto it = _lights.begin(); it != _lights.end(); it++) {
        (*it)->update(delta,_world);
        auto verts = (*it)->getVerts();
        auto indx = (*it)->getIndices();
        
        for (int i = 0; i < verts.size(); i++) {
            _vertData[_vertSize].pos = verts[i].pos*_scale;
            _vertData[_vertSize].color = verts[i].color;
            _vertData[_vertSize].frac = verts[i].frac;
            _vertSize++;
        }
        
        for (int j = 0; j < indx.size(); j++) {
            _indxData[_indxSize] = indx[j];
            _indxSize++;
        }
    }
    
}


//Needed to draw vbo
void RayHandler::pushToBuffer() {
    _vbo->loadVertexData(_vertData, _vertSize, GL_STREAM_DRAW);
    _vbo->loadIndexData(_indxData, _indxSize, GL_STREAM_DRAW);
    
    _indxSize = 0;
    _vertSize = 0;
}



void RayHandler::draw(const std::shared_ptr<SpriteBatch> &batch, const Mat4 &transform, Color4 tint) {
    
    batch->end();
    
    _vbo->bind();
    pushToBuffer();
    
    //TODO: Fix, multiply by transform possibly
    _shader->setUniformMat4("uPerspective", getScene()->getCamera()->getCombined());
    
    GLuint index = 0;
    GLuint size = 0;
    
    for (int i = 0; i < _lights.size(); i++) {
        size = (GLuint)_lights[i]->getIndices().size();
        
        if (_lights[i]->isPositional()) {
            _vbo->draw(GL_TRIANGLE_FAN, size, index);
        } else {
            _vbo->draw(GL_TRIANGLE_STRIP, size, index);
        }
        
        
        index += size;
    }
    
    _vbo->unbind();
    
    batch->begin();
    
}
