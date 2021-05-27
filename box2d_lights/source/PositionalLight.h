//
//  PositionalLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PositionalLight_h
#define PositionalLight_h

#include "Light.h"

namespace cugl {

    namespace b2dlights {

class PositionalLight : public Light {
protected:

    /** Shape information for this box */
    float _radius;
        
    
public:
#pragma mark -
#pragma mark Getters and Setters

    
    
    
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
    virtual bool init(const Vec2 pos, const int numRays, float radius);

    
    /**
     * Destroys this level, releasing all resources.
//     */
//    virtual ~PositionalLight(void);
//    
//    void dispose();
    
    
//    virtual void update(float delta) override;
    
#pragma mark -
#pragma mark Mesh Generation
    
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
    virtual bool contains(float x, float y) override;
    
    
//    bool attachToBody(
    
};

    }
    
}

#endif /* PositionalLight_h */
