//
//  PointLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#include "PositionalLight.h"

namespace cugl {

    namespace b2dlights {

class PointLight : public PositionalLight {
protected:

    
public:
#pragma mark -
#pragma mark Getters and Setters
    
    
    
#pragma mark -
#pragma mark Constructors and Destructors
    
    /**
     * Initializes a new light object of the given dimensions.
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
    virtual bool init(const Vec2 pos, int numRays, float radius) override;
    
    
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
    
    
    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~PointLight(void);
    
    
    void dispose();
    
    
#pragma mark -
#pragma mark Update
    
    
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
    
#pragma mark -
#pragma mark Light Mesh Generation
    
    
    bool calculateEndpoints();
    
    
};

    }

}

#endif /* PointLight_h */
