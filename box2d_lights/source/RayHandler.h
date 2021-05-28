//
//  RayHandler.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a capsule physics object. 
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
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/22/21

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

    namespace b2dlights {

class RayHandler : public scene2::SceneNode {
protected:
    
    std::shared_ptr<cugl::Shader> _shader;
    std::shared_ptr<cugl::VertexBuffer> _vbo;
    
    GLuint _maxVertices;
    GLuint _maxIndices;
    GLuint _vertSize;
    GLuint _indxSize;
    LightVert* _vertData;
    GLuint* _indxData;
    
    /** The bounds of this level in physics coordinates */
    Rect _bounds;
    
    //TODO: draw scale??
    float _scale;
    
    const bool gammaCorrection = false;
    const float gammaCorrectionParameter = 1.0f;
    
    bool _isDiffuse = false;
    
    cugl::Mat4 _combined;
    
    Color4 _ambientLight;
    
    
    //TODO: modify these
    std::vector<Vec2> _lightLocs;
    
    std::vector<std::shared_ptr<Light>> _lights;
    
    std::vector<bool> _disabledLights;
    
    
    bool _culling = true;
    bool _shadows = true;
    bool _blur = true;
    
    int _blurNum = 1;
    
    bool _customViewport = false;
    int _viewportX = 0;
    int _viewportY = 0;
    //TODO: set to screen
    int _viewportWidth;
    int _viewportHeight;
    
    int _lightRenderedLastFrame = 0;
    
    //camera matrix corners
    float x1, x2, y1, y2;
    
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    
public:
#pragma mark -
#pragma mark Constructors and DESTRUCTORS
        
    virtual bool init() override;
    
    /**
     * Creates a new game level with no source file.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<RayHandler> alloc() {
        std::shared_ptr<RayHandler> result = std::make_shared<RayHandler>();
        return (result->init() ? result : nullptr);
    }
    
    
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    RayHandler(void) : scene2::SceneNode() {};
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual ~RayHandler(void);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void dispose() override;
    
#pragma mark -
#pragma mark Getters and Setters
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void setCombinedMatrix(OrthographicCamera cam);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void setCombinedMatrix(cugl::Mat4 combined);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool intersect(float x, float y, float radius);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    std::shared_ptr<Light> getLight(int lid){
        return _lights[lid];
    }
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void setWorld(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {_world = world;}
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void setScale(float scale) {_scale = scale;}
    
    /**
     * Adds a new point light at the specified location.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    bool addPointLight(Vec2 vec, int numRays, float radius);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool addPointLight(float x, float y, int numRays, float radius) {
        return addPointLight(Vec2(x,y),numRays,radius);
    };
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool addConeLight(Vec2 vec, int numRays, float radius, float direction, float size);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool addDirectionalLight(Vec2 vec, int numRays, float direction);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void update(float delta);
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void pushToBuffer();
    
    /**
     * Initializes a new box object at the given point with no size.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    void draw(const std::shared_ptr<SpriteBatch> &batch, const Mat4 &transform, Color4 tint) override;
    
    
};
    
    }

}

#endif /* RayHandler_h */
