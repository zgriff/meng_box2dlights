//
//  Light.hpp
//  TestGame
//
//  Created by Zach Griffin on 4/14/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <cugl/cugl.h>

using namespace cugl;

class Light {
    
private:
    
protected:
    Color4 defaultColor = Color4(0.75f,0.75f,0.75f,0.75f);
//    float zeroColorBits = Color4.toFloatBits(0.f,0.f,0.f,0.f);
    int minRays = 3;
    
    
    Color4 color = Color4();
    Vec2 tmpPosition = Vec2();
    
    
    
    
    
//public:
    
};

#endif /* Light_hpp */
