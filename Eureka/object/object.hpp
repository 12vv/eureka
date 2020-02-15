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
//    Material *type = new Diffuse(Material::kDiffuse, Vec3f(0, 0, 1));
    Material *type = nullptr;
    float ior = 1; // index of refraction
//    Vec3f albedo = 0.5;
    Vec3f albedo = 0.5;
//    Vec3f color = Vec3f(0.5, 0.2, 0.1);
//    Vec3f color = 0.2;
    float Kd = 0.96; // phong model diffuse weight
    float Ks = 0.04; // phong model specular weight
    float n = 2;   // phong specular exponent
};

#endif /* object_h */
