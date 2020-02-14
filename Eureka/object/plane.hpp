//
//  plane.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/1.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef PLANE_HPP
#define PLANE_HPP

#include "object.hpp"

class Plane : public Object {
public:
    Plane(const Matrix44f &o2w, Material *m, const Vec3f &p, const Vec3f &n) : Object(o2w, m), position(p), normal(n) {
        o2w.multPtMatrix(p, position);
        normal.normalize();
    }
    bool intersect(const Ray &ray, float &tNear,
                   uint32_t &triIndex,
                   Vec2f &uv) const
    {
        float a = normal.dotProduct(ray.direction());
        if(a > 0)   return false;
        float b = -normal.dotProduct(ray.origin() - position);
        float t = b/a;
        if(t > 0 && t < tNear)  { tNear = t; return true; }
        return false;
//        float denom = normal.dotProduct(ray.direction());
//        if(denom < 1e-6){
//            Vec3f pl = position - ray.origin();
//            float t = pl.dotProduct(normal) / denom;
//            if(t > 0 && t < tNear)  { tNear = t; return true; }
//        }
//        return false;
    }
    // set normal and texture coordinates
    void getSurfaceData(const Vec3f &Phit,
                        const Vec3f &viewDirection,
                        const uint32_t &triIndex,   // not used for sphere
                        const Vec2f &uv,            // not used for sphere
                        Vec3f &Nhit,
                        Vec2f &textureCoordinates) const {
        Nhit = normal;

    }
    Vec3f position, normal;
    Vec3f albedo = Vec3f(0.225, 0.144, 0.144);
};

#endif /* plane_h */
