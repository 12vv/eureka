//
//  glossy.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/2.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef GLOSSY_HPP
#define GLOSSY_HPP

#include "material.hpp"

class Glossy : public Material {
public:
    Glossy(Type type, const Vec3f &c) : Material(type), color(c) {
        assert(type == kGlossy);
        name = "glossy";
    }
    Vec3f color;
};

#endif /* glossy_h */
