//
//  ray.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef RAY_HPP
#define RAY_HPP

#include "geometry.hpp"

class Ray {
public:
    Ray(){}
    Ray(const Vec3f &ori, const Vec3f &dir): ori(ori), dir(dir) {}
    Vec3f origin() const {return ori; }
    Vec3f direction() const {return dir; }
    Vec3f at(float t) const {return (ori + dir*t); }
    Vec3f ori, dir;
};


#endif /* ray_h */
