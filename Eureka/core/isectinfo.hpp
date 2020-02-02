//
//  insect.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/1.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef ISECTINFO_HPP
#define ISECTINFO_HPP

#include "../object/object.hpp"

class IsectInfo {
public:
    
    Object *hitObject = nullptr;
    float tNear = kInfinity;
    Vec2f uv;
    uint32_t index = 0;
};

#endif /* insect_h */
