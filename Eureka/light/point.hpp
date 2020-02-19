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

    Vec3f pos;
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
//        lightIntensity = intensity * color / (4 * M_PI * r2); // light attenuation follows inverse-square law
//        lightIntensity = 1;
        lightIntensity = intensity * color * 1e02 / r2;
//        std::cout << lightIntensity << std::endl;
    }

};

#endif /* point_h */
