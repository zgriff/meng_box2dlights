//
//  DirectionalLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef DirectionalLight_h
#define DirectionalLight_h

#include <stdio.h>
#include "Light.h"

namespace cugl {

    namespace b2dlights {

class DirectionalLight : public Light {
protected:

    
    float _radius;
    
    float _direction;
    
    
    float* _startX;
    float* _startY;
    
    
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
    virtual bool init(const Vec2 pos, const int numRays, const float direction, std::shared_ptr<cugl::physics2::ObstacleWorld> world);
    

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
    static std::shared_ptr<DirectionalLight> alloc(const Vec2 pos, int numRays, float direction, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        std::shared_ptr<DirectionalLight> result = std::make_shared<DirectionalLight>();
        return (result->init(pos, numRays, direction, world) ? result : nullptr);
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
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
    
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
    virtual bool calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
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
    bool calculateEndpoints(std::shared_ptr<cugl::physics2::ObstacleWorld> world);
    
    /**
     * Destroys this level, releasing all resources.
//     */
//    virtual ~PositionalLight(void);
//
//    void dispose();
    
#pragma mark -
#pragma mark Light Querying
    
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
    virtual bool contains(float x, float y) override;

    
    
};

    }
    
}

#endif /* DirectionalLight_h */
