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

RayHandler::RayHandler(void) : Asset(),
_root(nullptr)
{
    _bounds.size.set(1.0f, 1.0f);
}

RayHandler::~RayHandler(void) {
    dispose();
}


void RayHandler::dispose() {
    _root = nullptr;
    _world = nullptr;
}




