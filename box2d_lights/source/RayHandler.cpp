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


using namespace cugl;

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


bool RayHandler::addPointLight(Vec2 vec, int numRays, float radius) {
    auto light = PointLight::alloc(vec, numRays, radius);
    _world->addObstacle(light);
    light->setWorld(_world);
    light->calculateLightMesh();
    light->setDrawScale(_scale);
    
    light->setDebugColor(Color4::YELLOW);
    
    auto verts = light->getVerts();
    auto indx = light->getIndices();
    
    for (int i = 0; i < verts.size(); i++) {
        _vertData[_vertSize].pos = verts[i].pos;
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


void RayHandler::pushToBuffer() {
    _vbo->loadVertexData(_vertData, _vertSize, GL_STREAM_DRAW);
    _vbo->loadIndexData(_indxData, _indxSize, GL_STREAM_DRAW);
    
//    _indxSize = 0;
//    _vertSize = 0;
}



void RayHandler::draw(const std::shared_ptr<SpriteBatch> &batch, const Mat4 &transform, Color4 tint) {
    
    batch->end();
    
    _vbo->bind();
    pushToBuffer();
    
    GLuint index = 0;
    GLuint size = 0;
    
    _shader->setUniformMat4("uPerspective", transform);
    
    for (int i = 0; i < _lights.size(); i++) {
        size = _lights[i]->getNumRays()+1;
        _vbo->draw(GL_TRIANGLES, size, index);
        index += size;
    }
    
    _vbo->unbind();
    
    batch->begin();
    
}
