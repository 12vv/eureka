//
//  distant.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/30.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef DISTANT_HPP
#define DISTANT_HPP

#include "../utility/global.hpp"
#include "light.hpp"

class DistantLight : public Light {

    Vec3f dir;
    
public:
    DistantLight(const Matrix44f &l2w, const Vec3f &c=1, const float &i=1) : Light(l2w, c, i) {
        l2w.multVecMatrix(Vec3f(-1, 1, -1), dir);
        dir.normalize(); // in case the matrix scales the light
    }
    void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance) const {
        lightDir = dir;
        lightIntensity = color * intensity;
        distance = kInfinity;
    }
};

#endif /* distant_h */
