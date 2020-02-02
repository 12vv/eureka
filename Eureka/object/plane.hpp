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
    }
    bool intersect(const Ray &ray, float &tNear) const {
        float a = normal.dotProduct(ray.direction());
        if(a > 0)   return false;
        float b = -normal.dotProduct(ray.origin() - position);
        float t = b/a;
        if(t > 0 && t < tNear)  return true;
        return false;
    }
    void getSurfaceData(const Vec3f &Phit, Vec3f &Nhit, Vec2f &textureCoordinates) const {
//        Nhit = normal.normalize();
    }
    Vec3f position, normal;
};

#endif /* plane_h */
