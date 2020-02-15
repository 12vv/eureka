//
//  point.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/30.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef POINT_HPP
#define POINT_HPP

#include "light.hpp"

class PointLight : public Light {

public:
    PointLight(const Matrix44f &l2w, const Vec3f &p, const Vec3f &c = 1, const float &i = 1) : Light(l2w, c, i){
        l2w.multPtMatrix(p, pos);
    }
    void illuminate(const Vec3f &P, Vec3f &lightDir, Vec3f &lightIntensity, float &distance) const {
        lightDir = P - pos;
        float r2 = lightDir.norm();  // square distance
        distance = sqrtf(r2);
        lightDir.normalize();
//        lightDir.x /= distance, lightDir.y /= distance, lightDir.z /= distance;
        lightIntensity = intensity * color / (4 * M_PI * r2); // light attenuation follows inverse-square law
//        lightIntensity = intensity * color / (r2);
//        std::cout << lightDir << std::endl;
    }
    Vec3f pos;
};

#endif /* point_h */
