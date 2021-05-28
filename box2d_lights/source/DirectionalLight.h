//
//  DirectionalLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a directional light object. This type of light will
//  cover the entire ObstacleWorld it is passed, with the rays pointing in a
//  given angle.
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

#ifndef DirectionalLight_h
#define DirectionalLight_h

#include <stdio.h>
#include "Light.h"

namespace cugl {

    namespace b2dlights {

/**
 * Directional light model used for covering ObstacleWorld in a
 * light mesh.

 */
class DirectionalLight : public Light {
protected:

    /** The angle the rays point in degrees with 0 at x-axis */
    float _direction;
    
    /** Array containing x-coordinates of starting points for the raycasts */
    float* _startX;
    /** Array containing y-coordinates of starting points for the raycasts */
    float* _startY;
    
    
public:
#pragma mark -
#pragma mark Getters and Setters

    /**
     * Returns whether the light is positional or not for drawing purposes
     *
     * Positional lights use GL_TRIANGLE_FAN
     * Directional lights use GL_TRIANGLE_STRIP
     *
     * @return  true if positional, false if directional
     */
    virtual bool isPositional() override {return false;}
    
    
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
    virtual bool init(const int numRays, const float direction, std::shared_ptr<cugl::physics2::ObstacleWorld> world);
    

    /**
     * Returns a new directional light object with the provided parameters.
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
    static std::shared_ptr<DirectionalLight> alloc(int numRays, float direction, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        std::shared_ptr<DirectionalLight> result = std::make_shared<DirectionalLight>();
        return (result->init(numRays, direction, world) ? result : nullptr);
    }
    
    /**
     * Recalculates the light mesh from state and world changes
     *
     * This function needs to be called after altering information in this light object,
     * e.g. color, numRays, etc. It requires a shared pointer to the ObstacleWorld
     * for raycasting. Implementations of this method should NOT retain ownership
     * of the world. That is a tight coupling that we should avoid.
     *
     * @param  delta  Initial position in world coordinates
     * @param  world  Number of rays in the light
     *
     * @return a new point light object at the given point with no radius.
     */
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
    
    /**
     * Generates the light mesh based on the type of light and world snapshot.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  world  The current ObstacleWorld of the game.
     *
     * @return  true if the vector of LightVerts  was successfully populated, false otherwise.
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
     * Queries this light's mesh for the given point
     *
     * @param  x  The x-coordinate of the point to query
     * @param  y  The y-coordinate of the point to query
     *
     * @return  true if the point lies within the light's mesh
     */
    virtual bool contains(float x, float y) override;

    
    
};

    }
    
}

#endif /* DirectionalLight_h */
