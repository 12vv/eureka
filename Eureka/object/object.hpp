//
//  object.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <random>
#include "../utility/global.hpp"
#include "../geometry/ray.hpp"
#include "../material/material.hpp"
#include "../material/diffuse.hpp"

class Object {
public:
    Object(const Matrix44f &o2w, Material *m, const Vec3f &a) : objectToWorld(o2w), worldToObject(o2w.inverse()), type(m), albedo(a) {}
    virtual ~Object() {}
    virtual bool intersect(const Ray &, float &, uint32_t &, Vec2f &) const = 0;
    virtual void getSurfaceData(const Vec3f &,
                                const Vec3f &,
                                const uint32_t &,
                                const Vec2f &,
                                Vec3f &,
                                Vec2f &) const = 0;
    Matrix44f objectToWorld, worldToObject;
//    Material type = Material::kDiffuse;
    Material *type = nullptr;
    float ior = 1; // index of refraction
    Vec3f albedo = 0.18;

    float Kd = 0.8; // phong model diffuse weight
    float Ks = 0.2; // phong model specular weight
    float n = 10;   // phong specular exponent
};

#endif /* object_h */
