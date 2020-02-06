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
    Object(const Matrix44f &o2w, Material *m) : objectToWorld(o2w), worldToObject(o2w.inverse()), type(m) {}
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
    Vec3f albedo = 0.5;
//    Vec3f albedo = 0.18;
//    Vec3f color = Vec3f(0.5, 0.2, 0.1);
};

#endif /* object_h */
