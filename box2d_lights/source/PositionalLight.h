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
    
    
    float getRadius() {return _radius;}
    
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
