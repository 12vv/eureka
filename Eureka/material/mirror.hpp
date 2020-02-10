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
    Mirror(Type type) : Material(type) {
        assert(type == kReflect);
        name = "mirror";
    }

};

#endif /* mirror_h */
