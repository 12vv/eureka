//
//  diffuse.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/1.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include "material.hpp"

class Diffuse : public Material {
public:
    Diffuse(Type type, const Vec3f &c) : Material(type), color(c) {
        assert(type == kDiffuse);
        name = "diffuse";
    }
//    Vec3f sample(const std::vector<std::unique_ptr<Light>> &lights, const Vec3f &Phit, const Vec3f &Nhit, const Options &options, const std::vector<std::unique_ptr<Object>> &objects){
//        for(uint32_t i = 0; i < lights.size(); i++){
//            Vec3f lightDir, lightIntensity;
//            IsectInfo isectShad;
//            lights[i]->illuminate(Phit, lightDir, lightIntensity, isectShad.tNear);
//
////                    auto o = Phit + Nhit * options.bias;
////                    std::cout << o << std::endl;
////                    ray.ori = o;
//            Ray tmp(Phit + Nhit * options.bias, -lightDir);
//            bool vis = !trace(tmp, objects, isectShad);
////                    bool vis = trace(ray, objects, isectShad);  // ray here should add a bias !!!!!!!!!!!!!!!!!!!!
////                    std::cout << vis << std::endl;
//            hitColor += vis * isect.hitObject->albedo / M_PI * lights[i]->intensity * lights[i]->color * std::max(0.f, Nhit.dotProduct(-ray.dir));
////                    float angle = radian(45);
////                    float s = texCoordinates.x * cos(angle) - texCoordinates.y * sin(angle);
////                    float t = texCoordinates.y * cos(angle) + texCoordinates.x * sin(angle);
////                    float scaleS = 20, scaleT = 20;
////                    float pattern = (modulo(s * scaleS) < 0.5);
////                    hitColor += vis * pattern * lightIntensity * std::max(0.f, Phit.dotProduct(-lightDir));
//        }
//    }
    Vec3f color;
};

#endif /* diffuse_h */
