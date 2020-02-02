//
//  mirror.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/2.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef MIRROR_HPP
#define MIRROR_HPP

#include "material.hpp"

class Mirror : public Material {
public:
    Mirror(Type type, const Vec3f &c) : Material(type), color(c) {
        assert(type == kReflect);
        name = "mirror";
    }
    Vec3f color;
};

#endif /* mirror_h */
