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
#include "PositionalLight.h"

using namespace cugl;

class PointLight : public PositionalLight {
protected:

    
    float _radius;
//    float _direction;
//    float _colorF;
//    float _softShadowLength = 2.5f;
    

    
    
public:
#pragma mark -
#pragma mark Getters and Setters
    
    
    
#pragma mark -
#pragma mark Construct Destruct
    
    /**
     * Initializes a new wheel object of the given dimensions.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     * @param  numRays  Number of rays in the light
     * @param  radius  Radius of the point light circle
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2 pos, int numRays, float radius);
    
    
    /**
     * Returns a new point light object at the given point with no radius.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     * @param  numRays  Number of rays in the light
     * @param  radius  Radius of the point light circle
     *
     * @return a new point light object at the given point with no radius.
     */
    static std::shared_ptr<PointLight> alloc(const Vec2 pos, int numRays, float radius) {
        std::shared_ptr<PointLight> result = std::make_shared<PointLight>();
        return (result->init(pos, numRays, radius) ? result : nullptr);
    }
    
    
    virtual void calculateLightMesh();
    

    
    
    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~PointLight(void);
    
    void dispose();
    
    
};


#endif /* PointLight_h */
