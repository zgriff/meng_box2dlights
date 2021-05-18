//
//  Light.h
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>


using namespace cugl;

class LightVert {
public:
    Vec2 pos;
    Color4 color;
    float frac;
};


class Light : public cugl::physics2::WheelObstacle{
protected:
    
    std::shared_ptr<cugl::scene2::PolygonNode> _sceneNode;
    
    /** The Box2D world */
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    
    const Color4 _defaultColor = Color4::RED;
    
    Color4 _color;
    
    Vec2 _pos;
    
    float _drawscale;
    
    
    bool _staticLight = false;
    
    bool _culled = false;
    
    bool _dirty = true;
    
    bool _ignoreBody = false;
    
    int _numRays;
    int _numVerts;
    
    float _distance;
    float _direction;
    float _colorF;
    float _softShadowLength = 2.5f;
    
    int m_index = 0;
    std::vector<float> mx;
    std::vector<float> my;
    std::vector<float> f;
    
    std::vector<float> _segments;
    std::vector<SpriteVertex2> _segmentsMesh;
    
    std::vector<float>  _endx;
    std::vector<float>  _endy;
    
    
    
    Poly2 _poly;
    Poly2 _polyLastFrame;
    
    Mesh<SpriteVertex2> _lightMesh;
    Mesh<SpriteVertex2> _lightMeshLF;
    Mesh<SpriteVertex2> _softShadowMesh;
    Mesh<SpriteVertex2> _softShadowMeshLF;
        
    /** The blending equation for this texture */
    GLenum _blendEquation;
    /** The source factor for the blend function */
    GLenum _srcFactor;
    /** The destination factor for the blend function */
    GLenum _dstFactor;
    
    /** The gradient to use for this polygon */
    std::shared_ptr<Gradient> _gradient;
    /** The active gradient to use in the sprite batch pass */
    std::shared_ptr<Gradient> _spriteGrad;
    
    clock_t _updateTimer;
    clock_t _updateRate = 0.005f * CLOCKS_PER_SEC;
    
    
    
    
    
public:
#pragma mark -
#pragma mark Static Constructors
    
    /**
     * Initializes a new wheel object of the given dimensions.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  radius   The wheel radius
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2 pos, float radius);
    
    
    /**
     * Returns a new wheel object at the given point with no radius.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return a new wheel object at the given point with no radius.
     */
    static std::shared_ptr<Light> alloc(const Vec2 pos, int numRays, float radius) {
        std::shared_ptr<Light> result = std::make_shared<Light>();
        return (result->init(pos, 0.0f) ? result : nullptr);
    }
    
    /**
     * Returns a new wheel object of the given radius.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  radius   The wheel radius
     *
     * @return a new wheel object of the given radius.
     */
    static std::shared_ptr<Light> alloc(const Vec2 pos, float radius) {
        std::shared_ptr<Light> result = std::make_shared<Light>();
        return (result->init(pos,radius) ? result : nullptr);
    }

    
#pragma mark -
#pragma mark Getters and Setters
    
    void setTextures(const std::shared_ptr<cugl::Texture>& lightText);
    
    /**
     * Sets the ratio of the swap station sprite to the physics body
     *
     *
     * @param scale The ratio of the swap station sprite to the physics body
     */
    void setDrawScale(float scale);
    
    /**
     * Returns the ratio of the swap station sprite to the physics body
     *
     *
     * @return the ratio of the swap station sprite to the physics body
     */
    float getDrawScale() const { return _drawscale; }
    
    void setColor(Color4 color) {_color = color;}
    
    void setColor(float r, float g, float b, float a) {
        _color = Color4(r,g,b,a);
    }
    
    void setNumRays(int num) {_numRays = num;}
    
    int getNumRays() {return _numRays;}
    
    std::shared_ptr<cugl::physics2::ObstacleWorld> getWorld() {return _world;}
    
    void setWorld(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {_world = world;}
    
    
    std::shared_ptr<cugl::scene2::PolygonNode> getSceneNode() {return _sceneNode;}
    
    
//    void calculateLightMesh();
    
    Mesh<SpriteVertex2> getLightMesh() {return _lightMesh;}
    
    Mesh<SpriteVertex2> getShadowMesh() {return _softShadowMesh;}
    
    Poly2 getPoly() {return _poly;}
    
    Poly2 getPolyLastFrame() {return _polyLastFrame;}
    
    void setPolyLastFrame(Poly2 poly) {_polyLastFrame = poly;}
    
    std::vector<float> getSegments() {return _segments;}
    
    
#pragma mark -
#pragma mark Rendering
    
    /**
     * Draws this Node and all of its children with the given SpriteBatch.
     *
     * You almost never need to override this method.  You should override the
     * method draw(shared_ptr<SpriteBatch>,const Mat4&,Color4) if you need to
     * define custom drawing code.
     *
     * @param batch     The SpriteBatch to draw with.
     * @param transform The global transformation matrix.
     * @param tint      The tint to blend with the Node color.
     */
//    virtual void render(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint);
    
    
    void initializeLightMesh();
    
    virtual Mesh<SpriteVertex2> calculateLightMesh();
    
    std::function<float(b2Fixture*, const Vec2, const Vec2, float)> ray = [&](b2Fixture* fix, const Vec2 point, const Vec2 normal, float fraction) {
        if (fix->GetBody() == getBody()) return -1.0f;
        
        mx[m_index] = point.x;
        my[m_index] = point.y;
        f[m_index] = fraction;
        return fraction;
    };
    
    
    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~Light(void);
    
    void dispose();
    
    /*
     @param delta Timing values from parent loop
    */
    virtual void update(float delta) override;

    
};


#endif /* Light_h */
