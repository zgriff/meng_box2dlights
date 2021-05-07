//
//  RayHandler.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/4/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RayHandler_h
#define RayHandler_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "Light.h"


using namespace cugl;

class RayHandler : public Asset {
protected:
    /** The root node of this level */
    std::shared_ptr<scene2::SceneNode> _root;
    
    /** The bounds of this level in physics coordinates */
    Rect _bounds;
    
    const bool gammaCorrection = false;
    const float gammaCorrectionParameter = 1.0f;
    
    bool _isDiffuse = false;
    
    cugl::Mat4 _combined;
    
    Color4 _ambientLight;
    
    
    //TODO: modify these
    std::vector<Vec2> _lightLocs;
    
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
    
    void addObstacle(const std::shared_ptr<cugl::physics2::Obstacle>& obj,
                     const std::shared_ptr<cugl::scene2::SceneNode>& node, int zOrder);
    
public:
#pragma mark -
#pragma mark Constructors
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
        return (result->init("") ? result : nullptr);
    }
    
    
#pragma mark -
#pragma mark Getters and Setteres
    
    void setCombinedMatrix(OrthographicCamera cam);
    
    void setCombinedMatrix(cugl::Mat4 combined);
    
    // check if light is on screen
    bool intersect(float x, float y, float radius);
    
    
    
    
    Mesh<SpriteVertex2> calculateLightMesh(Light* light);
    
    
    
    RayHandler(void);
    
    virtual ~RayHandler(void);
    
    void dispose();
    
};

#endif /* RayHandler_h */
