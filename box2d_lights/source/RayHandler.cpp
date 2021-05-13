//
//  RayHandler.cpp
//  Box2DLights
//
//  Created by Zach Griffin on 5/4/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "RayHandler.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

#include <cugl/cugl.h>

using namespace cugl;



RayHandler::~RayHandler(void) {
    dispose();
}


void RayHandler::dispose() {
    _root = nullptr;
    _world = nullptr;
}



bool RayHandler::init(const std::shared_ptr<scene2::SceneNode>& root, const std::shared_ptr<cugl::physics2::ObstacleWorld> world, float scale) {
    _root = root;
    _world = world;
    _scale = scale;
    return true;
}


//void RayHandler::setRootNode(const std::shared_ptr<scene2::SceneNode> &root, const std::shared_ptr<cugl::physics2::ObstacleWorld> world, float scale) {
//
//
//}


bool RayHandler::addPointLight(Vec2 vec, int numRays, float radius) {
    auto light = PointLight::alloc(vec, numRays, radius);
    _world->addObstacle(light);
    light->setWorld(_world);
    light->calculateLightMesh();
    light->setDrawScale(_scale);
    
    light->setDebugColor(Color4::YELLOW);
    _root->addChild(light->getSceneNode(),1);
    _lights.push_back(light);
    
    return true;
}
