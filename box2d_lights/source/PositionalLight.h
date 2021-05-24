//
//  PositionalLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PositionalLight_h
#define PositionalLight_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "Light.h"

using namespace cugl;

class PositionalLight : public Light {
protected:

    
    float _radius;
//    float _direction;
//    float _colorF;
//    float _softShadowLength = 2.5f;
    

    
    
    
public:
#pragma mark -
#pragma mark Getters and Setters

    /**
     * Sets the ratio of the swap station sprite to the physics body
     *
     *
     * @param scale The ratio of the swap station sprite to the physics body
     */
//    void setDrawScale(float scale);
    
    /**
     * Returns the ratio of the swap station sprite to the physics body
     *
     *
     * @return the ratio of the swap station sprite to the physics body
     */
//    float getDrawScale() const { return _drawscale; }

    
    
    
#pragma mark -
#pragma mark Construct Destruct
    
    /**
//     * Initializes a new wheel object of the given dimensions.
//     *
//     * The scene graph is completely decoupled from the physics system.
//     * The node does not have to be the same size as the physics body. We
//     * only guarantee that the scene graph node is positioned correctly
//     * according to the drawing scale.
//     *
//     * @param  pos  Initial position in world coordinates
//     * @param  numRays  Number of rays in the light
//     * @param  radius  Radius of the point light circle
//     *
//     * @return  true if the obstacle is initialized properly, false otherwise.
//     */
//    virtual bool init(const Vec2 pos, int numRays);
//
//
//    /**
//     * Returns a new point light object at the given point with no radius.
//     *
//     * The scene graph is completely decoupled from the physics system.
//     * The node does not have to be the same size as the physics body. We
//     * only guarantee that the scene graph node is positioned correctly
//     * according to the drawing scale.
//     *
//     * @param  pos  Initial position in world coordinates
//     * @param  numRays  Number of rays in the light
//     * @param  radius  Radius of the point light circle
//     *
//     * @return a new point light object at the given point with no radius.
//     */
//    static std::shared_ptr<PositionalLight> alloc(const Vec2 pos, int numRays, float radius) {
//        std::shared_ptr<PositionalLight> result = std::make_shared<PositionalLight>();
//        return (result->init(pos, numRays, radius) ? result : nullptr);
//    }
    
    
//    std::function<float(b2Fixture*, const Vec2, const Vec2, float)> ray = [&](b2Fixture* fix, const Vec2 point, const Vec2 normal, float fraction) {
//        if (fix->GetBody() == getBody()) return -1.0f;
//
//        mx[m_index] = point.x;
//        my[m_index] = point.y;
//        f[m_index] = fraction;
//        return fraction;
//    };
    
    
    
    /**
     * Destroys this level, releasing all resources.
//     */
//    virtual ~PositionalLight(void);
//    
//    void dispose();
    
    
};

#endif /* PositionalLight_h */
