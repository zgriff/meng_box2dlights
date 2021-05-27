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
#include <vector>
#include <cmath>
#include <time.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Dynamics/b2Body.h>
#include <iostream>
#include <cugl/scene2/graph/CUWireNode.h>
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/physics2/CUObstacleWorld.h>

namespace cugl {

    namespace b2dlights {

    
class LightVert {
public:
    Vec2 pos;
    Color4 color;
    float frac;
};

class Light {
protected:
    /** Stores the body information for this shape */
    b2BodyDef _bodyinfo;
    /** The physics body for Box2D. */
    b2Body* _body;
    
    /** Number of decimal places to snap position of image to physics body */
    int _posSnap;
    /** Cache of factor to snap position of image to physics body */
    unsigned long _posFact;
    /** Number of decimal places to snap rotation of image to physics body */
    int _angSnap;
    /** Cache of factor to snap rotation of image to physics body */
    unsigned long _angFact;
    
    /** The wireframe parent for debugging. */
    std::shared_ptr<cugl::scene2::SceneNode> _scene;
    /** The wireframe node for debugging. */
    std::shared_ptr<cugl::scene2::WireNode> _debug;
    /** The wireframe color for debugging */
    Color4 _dcolor;
    /** A tag for debugging purposes */
    std::string _tag;
    
    std::shared_ptr<cugl::scene2::PolygonNode> _sceneNode;
    
    /** Shape information for this box */
    const Color4 _defaultColor = Color4::BLUE;
    /** Shape information for this box */
    Color4 _color;
    /** Shape information for this box */
    Vec2 _pos;
    /** Shape information for this box */
    float _drawscale;
    
    /** Shape information for this box */
    int _numRays;
    /** Shape information for this box */
    int _numVerts;
        
    /** Shape information for this box */
    int m_index = 0;
    /** Shape information for this box */
    std::vector<float> mx;
    /** Shape information for this box */
    std::vector<float> my;
    /** Shape information for this box */
    std::vector<float> f;

    /** Shape information for this box */
    std::vector<float>  _endX;
    /** Shape information for this box */
    std::vector<float>  _endY;
    
    /** Shape information for this box */
    std::vector<LightVert> _lightVerts;
    /** Shape information for this box */
    std::vector<Uint32> _lightIndx;
    
    /** Shape information for this box */
    bool _dirty;
    

    
#pragma mark -
#pragma mark Scene Graph Internals
    /**
     * Creates the outline of the physics fixtures in the debug wireframe
     *
     * The debug wireframe is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
    virtual void resetDebug() { }
    
    /**
     * Repositions the debug wireframe so that it agrees with the physics object.
     *
     * The debug wireframe is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a very different shape than
     * the texture (e.g. a circular shape attached to a square texture).
     */
     virtual void updateDebug();
    
    

#pragma mark -
#pragma mark Constructors
public:
    /**
     * Creates a new physics object at the origin.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
     * the heap, use one of the static constructors instead (in this case, in
     * one of the subclasses).
     */
    Light(void);
    
    /**
     * Deletes this physics object and all of its resources.
     *
     * We have to make the destructor public so that we can polymorphically
     * delete physics objects.
     *
     * A non-default destructor is necessary since we must release all
     * claims on scene graph nodes.
     */
    virtual ~Light();
    
    void dispose();
    
    /**
     * Initializes a new physics object at the origin.
     *
     * @return true if the Light is initialized properly, false otherwise.
     */
    virtual bool init() { return init(Vec2::ZERO, 50); }
    
    /**
     * Initializes a new light object of the given dimensions.
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
    virtual bool init(const Vec2 pos, const int numRays);
    
//    /**
//     * Initializes a new physics object at the given point
//     *
//     * @param  vec  Initial position in world coordinates
//     *
//     * @return true if the Light is initialized properly, false otherwise.
//     */
//    virtual bool init(const Vec2 vec);
    
#pragma mark -
#pragma mark Light Getters and Setters
    
    void setColor(Color4 color) {_color = color;}
    
    void setColor(float r, float g, float b, float a) {
        _color = Color4(r,g,b,a);
    }
    
    void setNumRays(int num) {_numRays = num;}
    
    int getNumRays() {return _numRays;}
    
    std::shared_ptr<cugl::scene2::PolygonNode> getSceneNode() {return _sceneNode;}
    
    Vec2 getPosition() {return _pos;}
    
    /**
     * Sets the ratio of the swap station sprite to the physics body
     *
     *
     * @param scale The ratio of the swap station sprite to the physics body
     */
    void setDrawScale(float scale) {
        _drawscale =  scale;
    };
    
    /**
     * Returns the ratio of the swap station sprite to the physics body
     *
     *
     * @return the ratio of the swap station sprite to the physics body
     */
    float getDrawScale() const { return _drawscale; }
    
    std::vector<LightVert> getVerts() {return _lightVerts;}
    
    std::vector<Uint32> getIndices() {return _lightIndx;}
    
#pragma mark -
#pragma mark Light Rendering
    
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
    
    virtual bool calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        return false;
    }
    
    std::function<float(b2Fixture*, const Vec2, const Vec2, float)> ray = [&](b2Fixture* fix, const Vec2 point, const Vec2 normal, float fraction) {
        
        mx[m_index] = point.x;
        my[m_index] = point.y;
        f[m_index] = fraction;
        return fraction;
    };
    
    
#pragma mark -
#pragma mark Light Querying
    
    virtual bool contains(float x, float y) {
        return false;
    }
    
    virtual bool contains(Vec2 vec) {
        return contains(vec.x, vec.y);
    }

    
#pragma mark -
#pragma mark BodyDef Methods
    /**
     * Returns the body type for Box2D physics
     *
     * If you want to lock a body in place (e.g. a platform) set this value to
     * STATIC. KINEMATIC allows the object to move (and some limited collisions),
     * but ignores external forces (e.g. gravity). DYNAMIC makes this is a
     * full-blown physics object.
     *
     * @return the body type for Box2D physics
     */
    virtual b2BodyType getBodyType() const {
        return (_body != nullptr ? _body->GetType() : _bodyinfo.type);
    }
    
    /**
     * Sets the body type for Box2D physics
     *
     * If you want to lock a body in place (e.g. a platform) set this value to
     * STATIC. KINEMATIC allows the object to move (and some limited collisions),
     * but ignores external forces (e.g. gravity). DYNAMIC makes this is a
     * full-blown physics object.
     *
     * @param value the body type for Box2D physics
     */
    virtual void setBodyType(b2BodyType value) {
        if (_body != nullptr) {
            _body->SetType(value);
        } else {
            _bodyinfo.type = value;
        }
    }
    
    /**
     * Returns the current position for this physics body
     *
     * This method converts from a Box2D vector type to a CUGL vector type. This
     * cuts down on the confusion between vector types.  It also means that
     * changes to the returned vector will have no effect on this object.
     *
     * @return the current position for this physics body
     */
    virtual Vec2 getPosition() const {
        if (_body != nullptr) {
            return Vec2(_body->GetPosition().x,_body->GetPosition().y);
        } else {
            return Vec2(_bodyinfo.position.x,_bodyinfo.position.y);
        }
    }
    
    /**
     * Sets the current position for this physics body
     *
     * This method converts from a CUGL vector type to a Box2D vector type. This
     * cuts down on the confusion between vector types.
     *
     * @param value  the current position for this physics body
     */
    virtual void setPosition(const Vec2 value) { setPosition(value.x,value.y); }
    
    /**
     * Sets the current position for this physics body
     *
     * @param x  the current x-coordinate for this physics body
     * @param y  the current y-coordinate for this physics body
     */
    virtual void setPosition(float x, float y) {
        if (_body != nullptr) {
            _body->SetTransform(b2Vec2(x,y),_body->GetAngle());
        } else {
            _bodyinfo.position.Set(x,y);
        }
    }
    
    /**
     * Returns the x-coordinate for this physics body
     *
     * @return the x-coordinate for this physics body
     */
    virtual float getX() const {
        return (_body != nullptr ? _body->GetPosition().x : _bodyinfo.position.x);
    }
    
    /**
     * Sets the x-coordinate for this physics body
     *
     * @param value  the x-coordinate for this physics body
     */
    virtual void setX(float value) {
        if (_body != nullptr) {
            _body->SetTransform(b2Vec2(value,_body->GetPosition().y),_body->GetAngle());
        } else {
            _bodyinfo.position.x = value;
        }
    }
    
    /**
     * Returns the x-coordinate for this physics body
     *
     * @return the x-coordinate for this physics body
     */
    virtual float getY() const {
        return (_body != nullptr ? _body->GetPosition().y : _bodyinfo.position.y);
    }
    
    /**
     * Sets the y-coordinate for this physics body
     *
     * @param value  the y-coordinate for this physics body
     */
    virtual void setY(float value) {
        if (_body != nullptr) {
            _body->SetTransform(b2Vec2(_body->GetPosition().y,value),_body->GetAngle());
        } else {
            _bodyinfo.position.y = value;
        }
    }
    
    /**
     * Returns the angle of rotation for this body (about the center).
     *
     * The value returned is in radians
     *
     * @return the angle of rotation for this body
     */
    virtual float getAngle() const {
        return (_body != nullptr ? _body->GetAngle() : _bodyinfo.angle);
    }
    
    /**
     * Sets the angle of rotation for this body (about the center).
     *
     * @param value  the angle of rotation for this body (in radians)
     */
    virtual void setAngle(float value) {
        if (_body != nullptr) {
            _body->SetTransform(_body->GetPosition(),value);
        } else {
            _bodyinfo.angle = value;
        }
    }
    
    /**
     * Returns the linear velocity for this physics body
     *
     * This method converts from a Box2D vector type to a CUGL vector type. This
     * cuts down on the confusion between vector types.  It also means that
     * changes to the returned vector will have no effect on this object.
     *
     * @return the linear velocity for this physics body
     */
    virtual Vec2 getLinearVelocity() const  {
        if (_body != nullptr) {
            return Vec2(_body->GetLinearVelocity().x,_body->GetLinearVelocity().y);
        } else {
            return Vec2(_bodyinfo.linearVelocity.x,_bodyinfo.linearVelocity.y);
        }
    }
    
    /**
     * Sets the linear velocity for this physics body
     *
     * This method converts from a CUGL vector type to a Box2D vector type. This
     * cuts down on the confusion between vector types.
     *
     * @param value  the linear velocity for this physics body
     */
    virtual void setLinearVelocity(const Vec2 value)  { setLinearVelocity(value.x,value.y); }
    
    /**
     * Sets the linear velocity for this physics body
     *
     * @param x  the x-coordinate of the linear velocity
     * @param y  the y-coordinate of the linear velocity
     */
    virtual void setLinearVelocity(float x, float y)  {
        if (_body != nullptr) {
            _body->SetLinearVelocity(b2Vec2(x,y));
        } else {
            _bodyinfo.linearVelocity.Set(x,y);
        }
    }
    
    /**
     * Returns the x-velocity for this physics body
     *
     * @return the x-velocity for this physics body
     */
    virtual float getVX() const  {
        if (_body != nullptr) {
            return _body->GetLinearVelocity().x;
        } else {
            return _bodyinfo.linearVelocity.x;
        }
    }
    
    /**
     * Sets the x-velocity for this physics body
     *
     * @param value  the x-velocity for this physics body
     */
    virtual void setVX(float value)  {
        if (_body != nullptr) {
            _body->SetLinearVelocity(b2Vec2(value,_body->GetLinearVelocity().y));
        } else {
            _bodyinfo.linearVelocity.x = value;
        }
    }
    
    /**
     * Returns the y-velocity for this physics body
     *
     * @return the y-velocity for this physics body
     */
    virtual float getVY() const {
        if (_body != nullptr) {
            return _body->GetLinearVelocity().y;
        } else {
            return _bodyinfo.linearVelocity.y;
        }
    }
    
    /**
     * Sets the y-velocity for this physics body
     *
     * @param value  the y-velocity for this physics body
     */
    virtual void setVY(float value)  {
        if (_body != nullptr) {
            _body->SetLinearVelocity(b2Vec2(_body->GetLinearVelocity().x,value));
        } else {
            _bodyinfo.linearVelocity.y = value;
        }
    }
    
    /**
     * Returns the angular velocity for this physics body
     *
     * The rate of change is measured in radians per step
     *
     * @return the angular velocity for this physics body
     */
    virtual float getAngularVelocity() const  {
        return (_body != nullptr ? _body->GetAngularVelocity() : _bodyinfo.angularVelocity);
    }
    
    /**
     * Sets the angular velocity for this physics body
     *
     * @param value the angular velocity for this physics body (in radians)
     */
    virtual void setAngularVelocity(float value)  {
        if (_body != nullptr) {
            _body->SetAngularVelocity(value);
        } else {
            _bodyinfo.angularVelocity = value;
        }
    }
    
    /**
     * Returns true if the body is active
     *
     * An inactive body not participate in collision or dynamics. This state is
     * similar to sleeping except the body will not be woken by other bodies and
     * the body's fixtures will not be placed in the broad-phase. This means the
     * body will not participate in collisions, ray casts, etc.
     *
     * @return true if the body is active
     */
    virtual bool isActive() const  {
        return (_body != nullptr ? _body->IsActive() : _bodyinfo.active);
    }
    
    /**
     * Sets whether the body is active
     *
     * An inactive body not participate in collision or dynamics. This state is
     * similar to sleeping except the body will not be woken by other bodies and
     * the body's fixtures will not be placed in the broad-phase. This means the
     * body will not participate in collisions, ray casts, etc.
     *
     * @param value  whether the body is active
     */
    virtual void setActive(bool value)  {
        if (_body != nullptr) {
            _body->SetActive(value);
        } else {
            _bodyinfo.active = value;
        }
    }
    
    /**
     * Returns true if the body is awake
     *
     * An sleeping body is one that has come to rest and the physics engine has
     * decided to stop simulating it to save CPU cycles. If a body is awake and
     * collides with a sleeping body, then the sleeping body wakes up. Bodies
     * will also wake up if a joint or contact attached to them is destroyed.
     * You can also wake a body manually.
     *
     * @return true if the body is awake
     */
    virtual bool isAwake() const  {
        return (_body != nullptr ? _body->IsAwake() : _bodyinfo.awake);
    }
    
    /**
     * Sets whether the body is awake
     *
     * An sleeping body is one that has come to rest and the physics engine has
     * decided to stop simulating it to save CPU cycles. If a body is awake and
     * collides with a sleeping body, then the sleeping body wakes up. Bodies
     * will also wake up if a joint or contact attached to them is destroyed.
     * You can also wake a body manually.
     *
     * @param value  whether the body is awake
     */
    virtual void setAwake(bool value)  {
        if (_body != nullptr) {
            _body->SetAwake(value);
        } else {
            _bodyinfo.awake = value;
        }
    }
    
    /**
     * Returns false if this body should never fall asleep
     *
     * An sleeping body is one that has come to rest and the physics engine has
     * decided to stop simulating it to save CPU cycles. If a body is awake and
     * collides with a sleeping body, then the sleeping body wakes up. Bodies
     * will also wake up if a joint or contact attached to them is destroyed.
     * You can also wake a body manually.
     *
     * @return false if this body should never fall asleep
     */
    virtual bool isSleepingAllowed() const  {
        return (_body != nullptr ? _body->IsSleepingAllowed() : _bodyinfo.allowSleep);
    }
    
    /**
     * Sets whether the body should ever fall asleep
     *
     * An sleeping body is one that has come to rest and the physics engine has
     * decided to stop simulating it to save CPU cycles. If a body is awake and
     * collides with a sleeping body, then the sleeping body wakes up. Bodies
     * will also wake up if a joint or contact attached to them is destroyed.
     * You can also wake a body manually.
     *
     * @param value  whether the body should ever fall asleep
     */
    virtual void setSleepingAllowed(bool value)  {
        if (_body != nullptr) {
            _body->SetSleepingAllowed(value);
        } else {
            _bodyinfo.allowSleep = value;
        }
    }
    
    /**
     * Returns true if this body is a bullet
     *
     * By default, Box2D uses continuous collision detection (CCD) to prevent
     * dynamic bodies from tunneling through static bodies. Normally CCD is not
     * used between dynamic bodies. This is done to keep performance reasonable.
     * In some game scenarios you need dynamic bodies to use CCD. For example,
     * you may want to shoot a high speed bullet at a stack of dynamic bricks.
     * Without CCD, the bullet might tunnel through the bricks.
     *
     * Fast moving objects in Box2D can be labeled as bullets. Bullets will
     * perform CCD with both static and dynamic bodies. You should decide what
     * bodies should be bullets based on your game design.
     *
     * @return true if this body is a bullet
     */
    virtual bool isBullet() const  {
        return (_body != nullptr ? _body->IsBullet() : _bodyinfo.bullet);
    }
    
    /**
     * Sets whether this body is a bullet
     *
     * By default, Box2D uses continuous collision detection (CCD) to prevent
     * dynamic bodies from tunneling through static bodies. Normally CCD is not
     * used between dynamic bodies. This is done to keep performance reasonable.
     * In some game scenarios you need dynamic bodies to use CCD. For example,
     * you may want to shoot a high speed bullet at a stack of dynamic bricks.
     * Without CCD, the bullet might tunnel through the bricks.
     *
     * Fast moving objects in Box2D can be labeled as bullets. Bullets will
     * perform CCD with both static and dynamic bodies. You should decide what
     * bodies should be bullets based on your game design.
     *
     * @param value  whether this body is a bullet
     */
    virtual void setBullet(bool value)  {
        if (_body != nullptr) {
            _body->SetBullet(value);
        } else {
            _bodyinfo.bullet = value;
        }
    }
    
    /**
     * Returns true if this body be prevented from rotating
     *
     * This is very useful for characters that should remain upright.
     *
     * @return true if this body be prevented from rotating
     */
    virtual bool isFixedRotation() const  {
        return (_body != nullptr ? _body->IsFixedRotation() : _bodyinfo.fixedRotation);
    }
    
    /**
     * Sets whether this body be prevented from rotating
     *
     * This is very useful for characters that should remain upright.
     *
     * @param value  whether this body be prevented from rotating
     */
    virtual void setFixedRotation(bool value)  {
        if (_body != nullptr) {
            _body->SetFixedRotation(value);
        } else {
            _bodyinfo.fixedRotation = value;
        }
    }
    
    /**
     * Returns the gravity scale to apply to this body
     *
     * This allows isolated objects to float.  Be careful with this, since
     * increased gravity can decrease stability.
     *
     * @return the gravity scale to apply to this body
     */
    virtual float getGravityScale() const  {
        return (_body != nullptr ? _body->GetGravityScale() : _bodyinfo.gravityScale);
    }
    
    /**
     * Sets the gravity scale to apply to this body
     *
     * This allows isolated objects to float.  Be careful with this, since
     * increased gravity can decrease stability.
     *
     * @param value  the gravity scale to apply to this body
     */
    virtual void setGravityScale(float value)  {
        if (_body != nullptr) {
            _body->SetGravityScale(value);
        } else {
            _bodyinfo.gravityScale = value;
        }
    }
    
    /**
     * Returns the linear damping for this body.
     *
     * Linear damping is use to reduce the linear velocity. Damping is different
     * than friction because friction only occurs with contact. Damping is not a
     * replacement for friction and the two effects should be used together.
     *
     * Damping parameters should be between 0 and infinity, with 0 meaning no
     * damping, and infinity meaning full damping. Normally you will use a
     * damping value between 0 and 0.1. Most people avoid linear damping because
     * it makes bodies look floaty.
     *
     * @return the linear damping for this body.
     */
    virtual float getLinearDamping() const  {
        return (_body != nullptr ? _body->GetLinearDamping() : _bodyinfo.linearDamping);
    }
    
    /**
     * Sets the linear damping for this body.
     *
     * Linear damping is use to reduce the linear velocity. Damping is different
     * than friction because friction only occurs with contact. Damping is not a
     * replacement for friction and the two effects should be used together.
     *
     * Damping parameters should be between 0 and infinity, with 0 meaning no
     * damping, and infinity meaning full damping. Normally you will use a
     * damping value between 0 and 0.1. Most people avoid linear damping because
     * it makes bodies look floaty.
     *
     * @param value  the linear damping for this body.
     */
    virtual void setLinearDamping(float value)  {
        if (_body != nullptr) {
            _body->SetLinearDamping(value);
        } else {
            _bodyinfo.linearDamping = value;
        }
    }
    
    /**
     * Returns the angular damping for this body.
     *
     * Angular damping is use to reduce the angular velocity. Damping is
     * different than friction because friction only occurs with contact.
     * Damping is not a replacement for friction and the two effects should be
     * used together.
     *
     * Damping parameters should be between 0 and infinity, with 0 meaning no
     * damping, and infinity meaning full damping. Normally you will use a
     * damping value between 0 and 0.1.
     *
     * @return the angular damping for this body.
     */
    virtual float getAngularDamping() const  {
        return (_body != nullptr ? _body->GetAngularDamping() : _bodyinfo.angularDamping);
    }
    
    /**
     * Sets the angular damping for this body.
     *
     * Angular damping is use to reduce the angular velocity. Damping is
     * different than friction because friction only occurs with contact.
     * Damping is not a replacement for friction and the two effects should be
     * used together.
     *
     * Damping parameters should be between 0 and infinity, with 0 meaning no
     * damping, and infinity meaning full damping. Normally you will use a
     * damping value between 0 and 0.1.
     *
     * @param value  the angular damping for this body.
     */
    virtual void setAngularDamping(float value)  {
        if (_body != nullptr) {
            _body->SetAngularDamping(value);
        } else {
            _bodyinfo.angularDamping = value;
        }
    }
    
    /**
     * Copies the state from the given body to the body def.
     *
     * This is important if you want to save the state of the body before
     * removing it from the world.
     */
    void setBodyState(const b2Body& body);

    
    
//#pragma mark -
//#pragma mark Garbage Collection
//    /**
//     * Returns true if our object has been flagged for garbage collection
//     *
//     * A garbage collected object will be removed from the physics world at
//     * the next time step.
//     *
//     * @return true if our object has been flagged for garbage collection
//     */
//    bool isRemoved() const { return _remove; }
//    
//    /**
//     * Sets whether our object has been flagged for garbage collection
//     *
//     * A garbage collected object will be removed from the physics world at
//     * the next time step.
//     *
//     * @param value  whether our object has been flagged for garbage collection
//     */
//    void markRemoved(bool value) { _remove = value; }
    
    
#pragma mark -
#pragma mark Physics Methods
    /**
     * Returns a (weak) reference to Box2D body for this obstacle.
     *
     * You use this body to add joints and apply forces. As a weak reference,
     * this physics obstacle does not transfer ownership of this body.  In
     * addition, the value may be a nullptr.
     *
     * @return a (weak) reference to Box2D body for this obstacle.
     */
    virtual b2Body* getBody() { return _body; }
    
    /**
     * Creates the physics Body(s) for this object, adding them to the world.
     *
     * Implementations of this method should NOT retain ownership of the
     * Box2D world. That is a tight coupling that we should avoid.
     *
     * @param world Box2D world to store body
     *
     * @return true if object allocation succeeded
     */
    virtual bool activatePhysics(b2World& world);
    
    /**
     * Destroys the physics Body(s) of this object if applicable.
     *
     * This removes the body from the Box2D world.
     *
     * @param world Box2D world that stores body
     */
    virtual void deactivatePhysics(b2World& world);
    

    
#pragma mark -
#pragma mark Update Methods
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * This method is called AFTER the collision resolution state. Therefore, it
     * should not be used to process actions or any other gameplay information.
     * Its primary purpose is to adjust changes to the fixture, which have to
     * take place after collision.
     *
     * In other words, this is the method that updates the scene graph.  If you
     * forget to call it, it will not draw your changes.
     *
     * @param delta Timing values from parent loop
     */
    virtual void update(float delta) {
        if (_scene) { updateDebug(); }
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
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        if (_scene) { updateDebug(); }
    }


#pragma mark -
#pragma mark Debugging Methods
    /**
     * Returns the physics object tag.
     *
     * A tag is a string attached to an object, in order to identify it in
     * debugging.
     *
     * @return the physics object tag.
     */
    std::string getName() const { return _tag; }
    
    /**
     * Sets the physics object tag.
     *
     * A tag is a string attached to an object, in order to identify it in
     * debugging.
     *
     * @param  value    the physics object tag
     */
    void setName(std::string value) { _tag = value; }

    /**
     * Returns a string representation of this physics object.
     *
     * This method converts the physics object into a string for debugging. By
     * default it shows the tag and position.  Other physics objects may want
     * to override this method for more detailed information.
     *
     * @return a string representation of this physics object
     */
    std::string toString() const;
    
    /**
     * Outputs this physics object to the given output stream.
     *
     * This function uses the toString() method to convert the physics object
     * into a string
     *
     * @param  os   the output stream
     * @param  obj  the physics object to ouput
     *
     * @return the output stream
     */
    friend std::ostream&   operator<<(std::ostream& os, const Light& obj);

    
#pragma mark -
#pragma mark Scene Graph Debugging Methods
    /**
     * Returns the color of the debug wireframe.
     *
     * The default color is white, which means that the objects will be shown
     * with a white wireframe.
     *
     * @return the color of the debug wireframe.
     */
    Color4 getDebugColor() const { return _dcolor; }
    
    /**
     * Sets the color of the debug wireframe.
     *
     * The default color is white, which means that the objects will be shown
     * with a white wireframe.
     *
     * @param color the color of the debug wireframe.
     */
    virtual void setDebugColor(Color4 color);
    
    /**
     * Returns the parent scene graph node for the debug wireframe
     *
     * The returned node is the parent coordinate space for drawing physics.
     * All debug nodes for physics objects are drawn within this coordinate
     * space.  Setting the visibility of this node to false will disable
     * any debugging.
     *
     * The wireframe will be drawn using physics coordinates, which is possibly
     * much smaller than your drawing coordinates (e.g. 1 Box2D unit = 1 pixel).
     * If you want the wireframes to be larger, you should scale the parent
     * parent coordinate space to match the rest of the application.
     *
     * This scene graph node is intended for debugging purposes only.  If
     * you want a physics body to update a proper texture image, you should
     * either use the method {@link update(float)} for subclasses or
     * {@link setListener} for decoupled classes.
     *
     * @return the scene graph node for the debug wireframe
     */
    scene2::SceneNode* getDebugScene() const { return _scene.get(); }
    
    /**
     * Returns the scene graph node for the debug wireframe
     *
     * The returned node draws a wireframe of the physics body. The wireframe
     * consists of the physics fixtures adjusted by the drawing scale.  The
     * debug node is positioned in the coordinate space of the parent scene.
     *
     * The wireframe will be drawn using physics coordinates, which is possibly
     * much smaller than your drawing coordinates (e.g. 1 Box2D unit = 1 pixel).
     * If you want the wireframes to be larger, you should scale the parent
     * parent coordinate space to match the rest of the application.
     *
     * This scene graph node is intended for debugging purposes only.  If
     * you want a physics body to update a proper texture image, you should
     * either use the method {@link update(float)} for subclasses or
     * {@link setListener} for decoupled classes.
     *
     * @return the scene graph node for the debug wireframe
     */
    scene2::WireNode* getDebugNode() const { return _debug.get(); }
    
    /**
     * Sets the parent scene graph node for the debug wireframe
     *
     * The given node is the parent coordinate space for drawing physics.
     * All debug nodes for physics objects are drawn within this coordinate
     * space.  Setting the visibility of this node to false will disable
     * any debugging.  Similarly, setting this value to nullptr will
     * disable any debugging.
     *
     * The wireframe will be drawn using physics coordinates, which is possibly
     * much smaller than your drawing coordinates (e.g. 1 Box2D unit = 1 pixel).
     * If you want the wireframes to be larger, you should scale the parent
     * parent coordinate space to match the rest of the application.
     *
     * This scene graph node is intended for debugging purposes only.  If
     * you want a physics body to update a proper texture image, you should
     * either use the method {@link update(float)} for subclasses or
     * {@link setListener} for decoupled classes.
     *
     * @param node  he parent scene graph node for the debug wireframe
     */
    virtual void setDebugScene(const std::shared_ptr<scene2::SceneNode>& node);
    
    /**
     * Returns true if the Light has a wireframe for debugging.
     *
     * This method will return false if there is no active parent scene
     * for the wireframe.
     *
     * @return true if the Light has a wireframe for debugging.
     */
    bool hasDebug() { return _scene != nullptr; }
    
};

    }

}
#endif /* Light_h */
