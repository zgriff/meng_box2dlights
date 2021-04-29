//
//  BlendFunc.hpp
//  TestGame
//
//  Created by Zach Griffin on 4/14/21.
//  Copyright © 2021 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BlendFunc_hpp
#define BlendFunc_hpp

#include <stdio.h>
#include <cugl/cugl.h>

using namespace cugl;

class BlendFunc {
protected:
    int default_sfactor;
    int default_dfactor;
    
    int sfactor;
    int dfactor;
    
public:
    BlendFunc();
    
    ~BlendFunc() { dispose(); }
    
    void dispose();
    
    bool init(int sfac, int dfac);
    
    
    void setFactors(int sfac, int dfac) {
        sfactor = sfac;
        dfactor = dfac;
    }
    
    
    void reset() {
        sfactor = default_sfactor;
        dfactor = default_dfactor;
    }
    
    void apply();
    
};

#endif /* BlendFunc_hpp */
