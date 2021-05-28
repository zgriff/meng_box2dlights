//
//  Light.h
//  Cornell University Game Library (CUGL)
//
//  This class implements a light physics object. A light is essentially a list
//  of Light vertices and indices (a light mesh) calculated through raycasting.
//  They use snapshots of the current physics world and the raycast callback
//  function from b2dworld to perform these calculations. The Light vertices
//  contain information about vertex position, color, and how far a ray was from
//  it's designated endpoint before hitting a fixture.
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
//  This file is based on CUObstacle and CUSimpleObstacle by Walker White, 2021
//
//  Author: Zach Griffin
//  Version: 5/28/21

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
 * Deletes this light object and all of its resources.
 *
 * We have to make the destructor public so that we can polymorphically
 * delete light objects.
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
 * Initializes a new light object with the given parameters.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos  Initial position in world coordinates ((0,0) by default)
 * @param  numRays  Number of rays used to calculate light mesh (100 by default)
 * @param  color  Color of the light (White by default)
 *
 * @return  true if the light is initialized properly, false otherwise.
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

