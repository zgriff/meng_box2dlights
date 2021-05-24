//
//  ConeLight.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/18/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ConeLight_h
#define ConeLight_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "PositionalLight.h"

using namespace cugl;

class ConeLight : public PositionalLight {
protected:

    
    float _radius;
    float _direction;
    float _size;
    
    
public:
#pragma mark -
#pragma mark Getters and Setters
    
//    void setColor(Color4 color) {_color = color;}
//    
//    void setColor(float r, float g, float b, float a) {
//        _color = Color4(r,g,b,a);
//    }
//        
//    
//    std::vector<LightVert> getVerts() {return _lightVerts;}
//    
//    std::vector<Uint32> getIndices() {return _lightIndx;}
    
    
    
#pragma mark -
#pragma mark Construct Destruct
    
    /**
     * Initializes a new wheel object of the given dimensions.
     *_indxSize    GLuint    150
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
    virtual bool init(const Vec2 pos, int numRays, float radius, float direction, float size);
    
    
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
    static std::shared_ptr<ConeLight> alloc(const Vec2 pos, int numRays, float radius, float direction, float size) {
        std::shared_ptr<ConeLight> result = std::make_shared<ConeLight>();
        return (result->init(pos, numRays, radius, direction, size) ? result : nullptr);
    }
    
    
    virtual void calculateLightMesh() ;
    
    
    
    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~ConeLight(void);
    
    void dispose();
    
    
};
#endif /* ConeLight_h */
