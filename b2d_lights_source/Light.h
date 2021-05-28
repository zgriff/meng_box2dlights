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

#ifndef Light_h
#define Light_h

#include <stdio.h>
#include <vector>
#include <cmath>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Dynamics/b2Body.h>
#include <cugl/scene2/graph/CUWireNode.h>
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/physics2/CUObstacleWorld.h>

namespace cugl {

    /**
     * The classes that hold all light physics of the game
     */
    namespace b2dlights {

/**
 * Simple class representing information needed to draw the light.
 */
class LightVert {
public:
    Vec2 pos;
    Color4 color;
    float frac;
};

/**
 * Base model class representing light sources.
 *
 * Instances represent light objects. There should be NO game
 * controlling logic code in the light object. That should reside in the
 * Controllers.
 *
 * This abstract class has no information on how the mesh is to be generated
 * and should never be instantiated directly. Instead, you should instantiate
 * either DirectionalLight or PositionalLight.
 * This class only exists to unify common functionality.
 *
 * Many of the method comments in this class are taken from CUObstacle and, by extension,
 * the Box2d manual by Erin Catto (2011).
 */
class Light {
    
protected:
    /** The default color for lights */
    const Color4 defaultColor = Color4::WHITE;
    
    /** Stores the body information for this shape */
    b2BodyDef _bodyinfo;
    /** The physics body for Box2D. */
    b2Body* _body;
    
    /** Number of decimal places to snap position of image to physics body */
    int _posSnap;
    /** Cache of factor to snap position of image to physics body */
    unsigned long _posFact;
    
    /** The wireframe parent for debugging. */
    std::shared_ptr<cugl::scene2::SceneNode> _scene;
    /** The wireframe node for debugging. */
    std::shared_ptr<cugl::scene2::WireNode> _debug;
    /** The wireframe color for debugging */
    Color4 _dcolor;
    /** A tag for debugging purposes */
    std::string _tag;
        
    /** The color for this light  */
    Color4 _color;
    
    /** The number of rays used in raycasting when calculating this light's mesh */
    int _numRays;
        
    /** The iterator used during raycasting */
    int m_index = 0;
    
    /** An array populated with the x-values of endpoints after raycasting */
    float* mx;
    /** An array populated with the y-values of endpoints after raycasting */
    float* my;
    /** An array populated with the fractions of raycasted endpoint / designated endpoint */
    float* f;
    
    /** An array populated with the x-values of ray endpoints before raycasting */
    float* _endX;
    /** An array populated with the y-values of ray endpoints before raycasting */
    float* _endY;
    
    /** A vector populated with each vertex that comprises the light mesh*/
    std::vector<LightVert> _lightVerts;
    /** A vector populated with indices used to for triangulation while drawing the mesh*/
    std::vector<Uint32> _lightIndx;
    
    /** Whether this light needs to recalculate it's start or endpoints */
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
    
    
public:

#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new light object.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
     * the heap, use one of the static constructors instead (in this case, in
     * one of the subclasses).
     */
    Light(void);
    
    /**
     * Deletes this light object and all of its resources.
     *
     * We have to make the destructor public so that we can polymorphically
     * delete light objects.
     *
     * A non-default destructor is necessary since we must release all
     * claims on scene graph nodes.
     */
    virtual ~Light();
    
    /**
     * Initializes a new white light object at the origin with 100 rays.
     *
     * @return true if the Light is initialized properly, false otherwise.
     */
    virtual bool init() { return init(Vec2::ZERO, 100); }
    
    /**
     * Initializes a new white light object at the given point with 100 rays.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return true if the light is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2 pos) {
        return init(pos, 100, defaultColor);
    }
    
    /**
     * Initializes a new white light object with the given parameters.
     *
     * @param  pos  Initial position in world coordinates
     * @param  numRays  Number of rays used to calculate light mesh (100 by default)
     *
     * @return true if the light is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2 pos, const int numRays) {
        return init(pos, numRays, defaultColor);
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
    virtual bool init(const Vec2 pos, const int numRays, const Color4 color);

    
#pragma mark -
#pragma mark Light Getters and Setters
    
    /**
     * Sets the color of the light to the given rgba value.
     *
     * @param  color  The desired color in Color4
     */
    void setColor(Color4 color) {_color = color;}
    
    /**
     * Sets the color of the light to the given rgba value.
     *
     * @param  r  The red value
     * @param  g  The green value
     * @param  b  The blue value
     * @param  a  The alpha value
     */
    void setColor(float r, float g, float b, float a) {
        _color = Color4(r,g,b,a);
    }
    
    /**
     * Sets the number of rays to use when calculating the light mesh.
     *
     * The higher the number,  the crisper the shape. Too high of a number
     * will cause performance to suffer.
     *
     * @param  num  Number of rays to use
     */
    void setNumRays(int num) {
        _numRays = num;
        
        mx = new float[_numRays];
        my = new float[_numRays];
        f = new float[_numRays];
        
        _endX = new float[_numRays];
        _endY = new float[_numRays];
        
        _dirty = true;
    }
    
    /**
     * Returns the number of rays used to calculate the light mesh.
     *
     * @return  Number of rays used
     */
    int getNumRays() {return _numRays;}
        
    /**
     * Returns a vector of vertices representing the light mesh.
     *
     * LightVert contains the position of a vertex, the color of the light,
     * and a fraction representing normalized distance from light source.
     *
     * @return  Vector representing the light mesh
     */
    std::vector<LightVert> getVerts() {return _lightVerts;}
    
    /**
     * Returns a vector of indices to be used when triangulating the mesh
     *
     * @return  Vector representing mesh indices
     */
    std::vector<Uint32> getIndices() {return _lightIndx;}
    
    /**
     * Returns whether the light is positional or not for drawing purposes
     *
     * Positional lights use GL_TRIANGLE_FAN
     * Directional lights use GL_TRIANGLE_STRIP
     *
     * @return  true if positional, false if directional
     */
    virtual bool isPositional() {return false;}
    
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
    virtual bool calculateLightMesh(std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        return false;
    }
    
    /**
     * Recalculates the ray endpoints in event of state changes
     *
     * Any time informatiom such as numRays or radius is changed,
     * the endpoints must be updated. Note: Endpoints are relative to the light's
     * position, and changes in position are calculated in calculateLightMesh.
     *
     * @return true if successfully able to update ray endpoints
     */
    virtual bool calculateEndpoints() {
        return false;
    }
    
    /**
     * Reports if a ray encountered any fixtures in its path -- Raycast callback
     *
     * This function is used as the callback when raycasting in the world. When raycasting,
     * one only to specify the start and end a ray. In its current implementation, this function
     * does not account for being attached to a physics body.
     *
     * The callback returns the fixture if the ray encounters one, the point that the ray gets
     * to before encountering a fixture, the normal vector of the fixture surface (if applicable),
     * and the fraction of raycasted ray magnitude / initial ray magnitude.
     */
    std::function<float(b2Fixture*, const Vec2, const Vec2, float)> ray = [&](b2Fixture* fix, const Vec2 point, const Vec2 normal, float fraction) {
        
        mx[m_index] = point.x;
        my[m_index] = point.y;
        f[m_index] = fraction;
        return fraction;
    };
    
    
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
    virtual bool contains(float x, float y) {
        return false;
    }
    
    /**
     * Queries this light's mesh for the given point.
     *
     * @param  vec  The Vec2 coordinate to query
     *
     * @return  true if the point lies within the light's mesh
     */
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
    
    
#pragma mark -
#pragma mark Update Methods
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
    virtual void update(float delta, std::shared_ptr<cugl::physics2::ObstacleWorld> world) {
        if (_scene) { updateDebug(); }
    }


#pragma mark -
#pragma mark Debugging Methods
    /**
     * Returns the light object tag.
     *
     * A tag is a string attached to an object, in order to identify it in
     * debugging.
     *
     * @return the light object tag.
     */
    std::string getName() const { return _tag; }
    
    /**
     * Sets the light object tag.
     *
     * A tag is a string attached to an object, in order to identify it in
     * debugging.
     *
     * @param  value    the light object tag
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
