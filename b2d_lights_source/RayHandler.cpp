//
//  RayHandler.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a ray handler node, which essentially serves as
//  a scene node for all of the lights. It utilizes a vertex buffer and a fairly
//  simple shader to draw all added lights to the scene.
//
//  This class uses our standard shared-pointer architecture.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  This file is based on CUSceneNode
//  by Walker White, 2021
//
//  Author: Zach Griffin
//  Version: 5/28/21

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

/**
 * Deletes this node, disposing all resources
 */
RayHandler::~RayHandler(void) {
    dispose();
}

/**
 * Disposes all of the resources used by this node.
 *
 * A disposed Node can be safely reinitialized. Any children owned by this
 * node will be released.  They will be deleted if no other object owns them.
 *
 * It is unsafe to call this on a Node that is still currently inside of
 * a scene graph.
 */
void RayHandler::dispose() {
    _lights.clear();
    _world = nullptr;
    _vbo = nullptr;
    _shader = nullptr;
}

/**
 * Initializes a rayhandler at the world origin.
 *
 * The rayhandler has both position and size (0,0).
 *
 * @return true if initialization was successful.
 */
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

/**
 * Instantiates a new point light object at the given point
 *
 * After allocating the light, you must call calculateLightMesh on the current
 * physics world before trying to draw.
 *
 * @param x  Initial x position in world coordinates
 * @param y  Initial y position in world coordinates
 * @param numRays  Number of rays for light to use when raycasting
 * @param radius  Radius of the point light
 *
 * @return  true if the light is instantiated properly, false otherwise.
 */
bool RayHandler::addPointLight(Vec2 vec, int numRays, float radius) {
    
    auto light = PointLight::alloc(vec, numRays, radius);

    light->calculateLightMesh(_world);
        
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

/**
 * Instantiates a new cone light object at the given point
 *
 * After allocating the light, you must call calculateLightMesh on the current
 * physics world before trying to draw.
 *
 * @param vec  Initial position in world coordinates
 * @param numRays  Number of rays for light to use when raycasting
 * @param radius  Radius of the cone light
 * @param direction  Direction of the cone light (where the center ray points) in degrees
 * @param size  Size of the cone light in degrees
 *
 * @return  true if the light is instantiated properly, false otherwise.
 */
bool RayHandler::addConeLight(Vec2 vec, int numRays, float radius, float direction, float size) {
    
    auto light = ConeLight::alloc(vec, numRays, radius, direction, size);
    
    light->calculateLightMesh(_world);

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

/**
 * Instantiates a new directional light object at the physics world
 *
 * After allocating the light, you must call calculateLightMesh on the current
 * physics world before trying to draw.
 *
 * @param numRays  Number of rays for light to use when raycasting
 * @param direction  Direction of the rays in degreees
 *
 * @return  true if the light is instantiated properly, false otherwise.
 */
bool RayHandler::addDirectionalLight(int numRays, float direction) {
    
    auto light = DirectionalLight::alloc(numRays, direction);
    
    light->calculateLightMesh(_world);
    
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

/**
 * Updates each light with the current physics world
 *
 * Each light must be passed a pointer to the world in order
 * to properly raycast.
 *
 * @param delta  Timing values from parent loop
 */
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

/**
 * Renders all lights to the scene graph.
 *
 * This function takes in the current active batch in order to end it while
 * rendering lights. It uses the current camera matrix as a perspective
 * for the shader. To draw, positional lights use a triangle fan and directional
 * lights use a triangle strip.
 *
 * @param batch  The current active scene batch
 * @param transform The global transformation matrix.
 * @param tint      The tint to blend with the Node color.
 */
void RayHandler::draw(const std::shared_ptr<SpriteBatch> &batch, const Mat4 &transform, Color4 tint) {
    
    batch->end();
    
    _vbo->bind();
    
    _vbo->loadVertexData(_vertData, _vertSize, GL_STREAM_DRAW);
    _vbo->loadIndexData(_indxData, _indxSize, GL_STREAM_DRAW);
    
    _indxSize = 0;
    _vertSize = 0;
    
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
