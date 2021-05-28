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

#ifndef RayHandler_h
#define RayHandler_h

#if CU_GL_PLATFORM == CU_GL_OPENGLES
    #define SHADER(A) ("#version 300 es\n#define CUGLES 1\n"+A)
#else
    #define SHADER(A) ("#version 330\n"+A)
#endif

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "Light.h"
#include "PointLight.h"
#include "ConeLight.h"
#include "DirectionalLight.h"

#define DEFAULT_CAPACITY  8192


namespace cugl {
    /**
     * The classes that hold all light physics of the game
     */
    namespace b2dlights {
    
/**
 * This is a scene graph node used to instantiate and draw various
 * light sources to a scene.
 */
class RayHandler : public scene2::SceneNode {
protected:
    
    /** The shader used to render the lights */
    std::shared_ptr<cugl::Shader> _shader;
    /** The vertex buffer used to pass light vertex data to the shader */
    std::shared_ptr<cugl::VertexBuffer> _vbo;
    
    /** Initialization size for vertex array */
    GLuint _maxVertices;
    /** Initialization size for index array */
    GLuint _maxIndices;
    /** The number of vertices, used for iteration */
    GLuint _vertSize;
    /** The number of indices, used for iteration */
    GLuint _indxSize;
    /** An array containing the vertex data for all lights */
    LightVert* _vertData;
    /** An array containing the vertex index data for all lights */
    GLuint* _indxData;
    
    /** The drawscale of the scene */
    float _scale;
    
    /** A vector containing all of the lights in the scene */
    std::vector<std::shared_ptr<Light>> _lights;
    /** The current physics world */
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    
public:
#pragma mark -
#pragma mark Constructors and DESTRUCTORS
    
    /**
     * Initializes a rayhandler at the world origin.
     *
     * The rayhandler has both position and size (0,0).
     *
     * @return true if initialization was successful.
     */
    virtual bool init() override;
    
    /**
     * Returns a newly allocated rayhandler at the world origin.
     *
     * The node has both position and size (0,0). 
     *
     * @return a newly allocated rayhandler
     */
    static std::shared_ptr<RayHandler> alloc() {
        std::shared_ptr<RayHandler> result = std::make_shared<RayHandler>();
        return (result->init() ? result : nullptr);
    }
    
    /**
     * Creates an uninitialized rayhandler.
     *
     * You must initialize this rayhandler before use.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a Node on the
     * heap, use one of the static constructors instead.
     */
    RayHandler(void) : scene2::SceneNode() {};
    
    /**
     * Deletes this node, disposing all resources
     */
    virtual ~RayHandler(void);
    
    /**
     * Disposes all of the resources used by this node.
     *
     * A disposed Node can be safely reinitialized. Any children owned by this
     * node will be released.  They will be deleted if no other object owns them.
     *
     * It is unsafe to call this on a Node that is still currently inside of
     * a scene graph.
     */
    void dispose() override;
    
#pragma mark -
#pragma mark Getters and Setters
    
    /**
     * Returns a pointer to a light in the scene.
     *
     * @param  lid  The id of the light (currently just order instantiated in)
     *
     * @return  a pointer to a light
     */
    std::shared_ptr<Light> getLight(int lid){
        return _lights[lid];
    }
    
    /**
     * Sets the current physics world of the scene.
     *
     * The world must be passed into each light when creating the mesh.
     * We want to be sure to decouple the light from the world.
     *
     * @param  world  The current physics world
     */
    void setWorld(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        _world = world;
    }
    
    /**
     * Sets the drawing scale of this scene node.
     *
     * The drawing scale is used when converting the light mesh coordinates
     * to vertex data to be rendered by the shader.
     *
     * @param scale  Drawing scale of the scene (box2d to scene coordinates)
     */
    void setScale(float scale) {_scale = scale;}
    
    
#pragma mark -
#pragma mark Light Creators
    
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
    bool addPointLight(Vec2 vec, int numRays, float radius);
    
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
    bool addPointLight(float x, float y, int numRays, float radius) {
        return addPointLight(Vec2(x,y),numRays,radius);
    };
    
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
    bool addConeLight(Vec2 vec, int numRays, float radius, float direction, float size);
    
    /**
     * Instantiates a new cone light object at the given point
     *
     * After allocating the light, you must call calculateLightMesh on the current
     * physics world before trying to draw.
     *
     * @param x  Initial x position in world coordinates
     * @param y  Initial y position in world coordinates
     * @param numRays  Number of rays for light to use when raycasting
     * @param radius  Radius of the cone light
     * @param direction  Direction of the cone light (where the center ray points) in degrees
     * @param size  Size of the cone light in degrees
     *
     * @return  true if the light is instantiated properly, false otherwise.
     */
    bool addConeLight(float x, float y, int numRays, float radius, float direction, float size) {
        return addConeLight(Vec2(x,y),numRays,radius,direction,size);
    };
    
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
    bool addDirectionalLight(int numRays, float direction);
    
    
#pragma mark -
#pragma mark Update
    
    /**
     * Updates each light with the current physics world
     *
     * Each light must be passed a pointer to the world in order
     * to properly raycast.
     *
     * @param delta  Timing values from parent loop
     */
    void update(float delta);
    
#pragma mark -
#pragma mark Drawing
    
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
    void draw(const std::shared_ptr<SpriteBatch> &batch, const Mat4 &transform, Color4 tint) override;
    
    
};
    
    }

}

#endif /* RayHandler_h */
