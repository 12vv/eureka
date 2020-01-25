//
//  sphere.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include "sphere.hpp"

class Sphere {
public:
    Sphere(const Vec3f &c, const float &r): radius(r), radius2(r*r), center(c) {}
    
    float radius, radius2;
    Vec3f center;
};


#endif /* sphere_h */
