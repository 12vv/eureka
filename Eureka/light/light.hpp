//
//  light.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/29.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef light_h
#define light_h

#include "../geometry/geometry.hpp"

class Light {
public:
    Light(const Matrix44f &l2w, const Vec3f &c = 1, const float &i = 1) : LightToWorld(l2w), color(c), intensity(i) {}
    virtual ~Light() {}
    virtual void illuminate(const Vec3f &P, Vec3f &, Vec3f &, float &) const = 0;
    Vec3f color;
    float intensity;
    Matrix44f LightToWorld;
};

#endif /* light_h */
