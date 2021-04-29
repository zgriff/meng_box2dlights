//
//  BlendFunc.cpp
//  TestGame
//
//  Created by Zach Griffin on 4/14/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#include "BlendFunc.hpp"

BlendFunc::BlendFunc() :
    default_sfactor(0),
    default_dfactor(0),
    sfactor(0),
    dfactor(0)
    {}

void BlendFunc::dispose() {}

bool BlendFunc::init(int sfac, int dfac) {
    default_sfactor = sfac;
    default_dfactor = dfac;
    sfactor = sfac;
    dfactor = dfac;
    return true;
}

void BlendFunc::apply() {
    
}
