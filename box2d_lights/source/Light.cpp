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
    _lightVerts.clear();
    _lightIndx.clear();
}

/**
 * Initializes a new Light object at the given point
 *
 * @param  vec  Initial position in world coordinates
 * @param  numRays  Number of rays used to calculate light mesh
 * @param  color  Color of the light - White by default
 *
 * @return  true if the Light is initialized properly, false otherwise.
 */
bool Light::init(const Vec2 pos, const int numRays, const Color4 color) {
    
    // Allocate the body information
    _bodyinfo.position.Set(pos.x,pos.y);
    // Objects are physics objects unless otherwise noted
    _bodyinfo.type = b2_dynamicBody;
    
    _numRays = numRays;
    _color = color;
    
    mx = new float[_numRays];
    my = new float[_numRays];
    f = new float[_numRays];
    
    _endX = new float[_numRays];
    _endY = new float[_numRays];
    
    _dirty = true;
    
    return true;
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
    
    // Positional snap
    if (_posSnap >= 0) {
        pos.x = floor((pos.x*_posFact+0.5f)/_posFact);
        pos.y = floor((pos.y*_posFact+0.5f)/_posFact);
    }
    
    _debug->setPosition(pos);
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

