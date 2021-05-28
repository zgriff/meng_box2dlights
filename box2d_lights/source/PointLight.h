//
//  PointLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a point light object. A point light is a light
//  source at a given point with rays extending out in 360 degrees. It can
//  be thought of as a cone light with coneDegreee set to 360.0f.
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
//  This file is based on CUObstacle, CUSimpleObstacle, and CUBoxObstacle
//  by Walker White, 2021
//
//  Author: Zach Griffin
//  Version: 5/28/21

#ifndef PointLight_h
#define PointLight_h

#include "PositionalLight.h"

namespace cugl {

    namespace b2dlights {

/**
 * Point light model used for a single point, 360 degree light source 
 */
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
    
    /**
     * Recalculates the light mesh from state and world changes
     *
     * This function needs to be called after altering information in this light object,
     * e.g. color, numRays, etc. It requires a shared pointer to the ObstacleWorld
     * for raycasting. Implementations of this method should NOT retain ownership of the
     * Box2D world. That is a tight coupling that we should avoid.
     *
     * @param  delta  Initial position in world coordinates
     * @param  world  Number of rays in the light
     *
     * @return a new point light object at the given point with no radius.
     */
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
    
#pragma mark -
#pragma mark Light Mesh Generation
    
    /**
     * Recalculates the ray endpoints from state changes
     *
     * This function needs to be called after altering information in this light object,
     * e.g. color, numRays, etc. It requires a shared pointer to the ObstacleWorld
     * for raycasting. Implementations of this method should NOT retain ownership of the
     * Box2D world. That is a tight coupling that we should avoid.
     *
     * @return true if successfully able to update ray endpoints
     */
    bool calculateEndpoints();
    
    
};

    }

}

#endif /* PointLight_h */
