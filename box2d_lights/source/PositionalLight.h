//
//  PositionalLight.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a positional light object. It serves to provide a
//  uniform interface for all location based lights. However, it still cannot
//  be instantiated directly, as the correct instantiation depends on the shape
//  of the light. See PointLight and ConeLight for concrete examples.
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

#ifndef PositionalLight_h
#define PositionalLight_h

#include "Light.h"

namespace cugl {

    namespace b2dlights {

/**
 * Positional light model used for a single point light source
 */
class PositionalLight : public Light {
    
protected:

    /** The radius of this light (cone or point) */
    float _radius;
    
    
public:
    
#pragma mark -
#pragma mark Getters and Setters
    
    /**
     * Returns the radius of this light, i.e. the magnitude of its rays
     *
     * @return a new point light object at the given point with no radius.
     */
    float getRadius() {return _radius;}
    
    /**
     * Sets the radius of this light.
     *
     * The update method must be called after this method in order
     * for any changes to take effect.
     *
     * @param  radius  Radius of the light, i.e. magnitude of its rays
     */
    void setRadius(float radius) {
        _radius = radius;
        _dirty = true;
    }
    
    /**
     * Returns whether the light is positional or not for drawing purposes
     *
     * Positional lights use GL_TRIANGLE_FAN
     * Directional lights use GL_TRIANGLE_STRIP
     *
     * @return  true if positional, false if directional
     */
    virtual bool isPositional() override {return true;}
    
    
#pragma mark -
#pragma mark Constructors
    
    /**
     * Initializes a new positional light object with the given parameters.
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
     * @return  true if the light is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2 pos, const int numRays, float radius);
    
    
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
     * Generates the light mesh based on the type of light and world snapshot.
     *
     * This method uses the position of the light and the calculated ray
     * endpoints raycast in the provided world. If a ray hits a fixture, we record
     * the new ray endpoint and the fraction of the magnitude of the original ray
     * over the magnitude of the new ray. Implementations of this method should
     * not retain ownership of the world as that is tight coupling.
     *
     * @param  world  The current ObstacleWorld of the game.
     *
     * @return  true if the vector of LightVerts  was successfully populated, false otherwise.
     */
    virtual bool calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) override;
    
#pragma mark -
#pragma mark Light Mesh Querying
    
    /**
     * Queries this light's mesh for the given point
     *
     * @param  x  The x-coordinate of the point to query
     * @param  y  The y-coordinate of the point to query
     *
     * @return  true if the point lies within the light's mesh
     */
    virtual bool contains(float x, float y) override;
    
    
//    bool attachToBody(
    
};

    }
    
}

#endif /* PositionalLight_h */
