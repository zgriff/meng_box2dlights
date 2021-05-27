//
//  Light.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/3/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "Light.h"
#include <cmath>
#include <math.h>
#include <Box2D/Dynamics/b2World.h>

using namespace cugl::b2dlights;

#pragma mark -
#pragma mark Constructor


Light::Light() :
_scene(nullptr),
_debug(nullptr)
{ }

/**
 * Deletes this physics object and all of its resources.
 *
 * We have to make the destructor public so that we can polymorphically
 * delete physics objects.
 *
 * A non-default destructor is necessary since we must release all
 * claims on scene graph nodes.
 */
Light::~Light() {
    setDebugScene(nullptr);
    dispose();
}


void Light::dispose() {
    mx.clear();
    my.clear();
    f.clear();
    _endX.clear();
    _endY.clear();
    _lightVerts.clear();
    _lightIndx.clear();
}

/**
 * Initializes a new physics object at the given point
 *
 * @param  vec  Initial position in world coordinates
 *
 * @return  true if the Light is initialized properly, false otherwise.
 */
bool Light::init(const Vec2 pos, const int numRays) {
    // Object has yet to be deactivated
//    _remove = false;
    
    // Allocate the body information
    _bodyinfo.awake  = true;
    _bodyinfo.allowSleep = true;
    _bodyinfo.gravityScale = 1.0f;
    _bodyinfo.position.Set(pos.x,pos.y);
    // Objects are physics objects unless otherwise noted
    _bodyinfo.type = b2_dynamicBody;
    
    _pos = pos;
    _numRays = numRays;
    _color = _defaultColor;
    
    return true;
}

/**
 * Copies the state from the given body to the body def.
 *
 * This is important if you want to save the state of the body before removing
 * it from the world.
 */
void Light::setBodyState(const b2Body& body) {
    _bodyinfo.type   = body.GetType();
    _bodyinfo.angle  = body.GetAngle();
    _bodyinfo.active = body.IsActive();
    _bodyinfo.awake  = body.IsAwake();
    _bodyinfo.bullet = body.IsBullet();
    _bodyinfo.position.Set(body.GetPosition().x,body.GetPosition().y);
    _bodyinfo.linearVelocity.Set(body.GetLinearVelocity().x,body.GetLinearVelocity().y);
    _bodyinfo.allowSleep = body.IsSleepingAllowed();
    _bodyinfo.fixedRotation = body.IsFixedRotation();
    _bodyinfo.gravityScale  = body.GetGravityScale();
    _bodyinfo.angularDamping = body.GetAngularDamping();
    _bodyinfo.linearDamping  = body.GetLinearDamping();
}

#pragma mark -
#pragma mark Physics Methods

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
bool Light::activatePhysics(b2World& world) {
    // Make a body, if possible
    _bodyinfo.active = true;
    _body = world.CreateBody(&_bodyinfo);
    _body->SetUserData((void*)this);
    
    // Only initialize if a body was created.
    if (_body != nullptr) {
        return true;
    }
    
    _bodyinfo.active = false;
    return false;
}

/**
 * Destroys the physics Body(s) of this object if applicable.
 *
 * This removes the body from the Box2D world.
 *
 * @param world Box2D world that stores body
 */
void Light::deactivatePhysics(b2World& world) {
    // Should be good for most (simple) applications.
    if (_body != nullptr) {
        // Snapshot the values
        setBodyState(*_body);
        world.DestroyBody(_body);
        _body = nullptr;
        _bodyinfo.active = false;
    }
}



#pragma mark -
#pragma mark Scene Graph Methods
/**
 * Sets the color of the debug wireframe.
 *
 * The default color is white, which means that the objects will be shown
 * with a white wireframe.
 *
 * @param color the color of the debug wireframe.
 */
void Light::setDebugColor(Color4 color) {
    _dcolor = color;
    if (_debug) {
        _debug->setColor(color);
    }
}

/**
 * Sets the scene graph node for drawing purposes.
 *
 * The scene graph is completely decoupled from the physics system.  The node
 * does not have to be the same size as the physics body. We only guarantee
 * that the node is positioned correctly according to the drawing scale.
 *
 * @param value  the scene graph node for drawing purposes.
 *
 * @retain  a reference to this scene graph node
 * @release the previous debug graph node used by this object
 */
void Light::setDebugScene(const std::shared_ptr<scene2::SceneNode>& node) {
    // Release the node if we have one previously
    if (_scene != nullptr) {
        if (_debug != nullptr && _debug->getParent() != nullptr) {
            _scene->removeChild(_debug);
        }
        _scene = nullptr;
    }
    if (node != nullptr) {
        _scene = node;
        resetDebug();
        updateDebug();
    }
}

/**
 * Repositions the scene node so that it agrees with the physics object.
 *
 * By default, the position of a node should be the body position times
 * the draw scale.  However, for some obstacles (particularly complex
 * obstacles), it may be desirable to turn the default functionality
 * off.  Hence we have made this virtual.
 */
void Light::updateDebug() {
    CUAssertLog(_scene, "Attempt to reposition a wireframe with no parent");
    Vec2 pos = getPosition();
    float angle = getAngle();
    
    // Positional snap
    if (_posSnap >= 0) {
        pos.x = floor((pos.x*_posFact+0.5f)/_posFact);
        pos.y = floor((pos.y*_posFact+0.5f)/_posFact);
    }
    // Rotational snap
    if (_angSnap >= 0) {
        angle = (float)(180*angle/M_PI);
        angle = floor((angle*_angFact+0.5f)/_angFact); // Formula is for degrees
        angle = (float)(M_PI*angle/180);
    }
    
    _debug->setPosition(pos);
    _debug->setAngle(angle);
}


#pragma mark -
#pragma Debugging Methods

/**
 * Returns a string representation of this physics object.
 *
 * This method converts the physics object into a string for debugging.  By
 * default it shows the tag and position.  Other physics objects may want to
 * override this method for more detailed information.
 *
 * @return a string representation of this physics object
 */
std::string Light::toString() const {
    std::stringstream ss;
    Vec2 p = getPosition();
    ss << "[Light " << _tag << ": (" << p.x << "," << p.y << "), ";
    ss << (isActive() ? "active ]" : "inactive ]");
    return ss.str();
}

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
std::ostream& operator<<(std::ostream& os, const Light& obj) {
    os << obj.toString();
    return os;
}

