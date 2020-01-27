//
//  sphere.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include "object.hpp"

class Sphere : public Object {
public:
    Sphere(const Vec3f &c, const float &r): radius(r), radius2(r*r), center(c) {}
    // Ray-sphere intersection test
    // @param ori: the ray origin
    // @param dir: the ray direction
    // @param t: the distance from the ray origin to tht intersection point
    bool intersect(const Vec3f &ori, const Vec3f &dir, float &t) const {
        float t0, t1; // solutions for t if the ray intersects
        
        // dot((p(t)-C), (p(t)-C)) = R^2 ;
        // p(t) = origin + t * direction ;
        // t^2 * dot(dir, dir) + 2*t*dot(dir, (origin-C)) + dot((origin-C), (origin-C)) - R^2 = 0 ;
        Vec3f L = ori - center;
        float a = dir.dotProduct(dir);
        float b = 2 * dir.dotProduct(L);
        float c = L.dotProduct(L) - radius2;
        if (!solveQuadratic(a, b, c, t0, t1)) return false;
        
        if (t0 > t1) std::swap(t0, t1);
        
        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return false; // both t0 and t1 are negative
        }
        
        t = t0;
        
        return true; 
    }
 
    // set normal and texture coordinates
    void getSurfaceData(const Vec3f &Phit, Vec3f &Nhit, Vec2f &tex) const {
        Nhit = Phit - center;
        Nhit.normalize();
        // remap from range[-pi, pi] to range[0, 1]
        tex.x = (1 + atan2(Nhit.z, Nhit.x) / M_PI) * 0.5;
        tex.y = acosf(Nhit.y) / M_PI;
    }
    
    float radius, radius2;
    Vec3f center;
};


#endif /* sphere_h */
