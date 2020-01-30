//
//  object.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef object_h
#define object_h

#include <random>
#include "../utility/global.hpp"
#include "../geometry/ray.hpp"

//std::random_device rd;
//std::mt19937 gen(rd());
//std::uniform_real_distribution<> dis(0, 1);

//bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
//{
//    float discr = b * b - 4 * a * c;
//    if (discr < 0) return false;
//    else if (discr == 0) {
//        x0 = x1 = - 0.5 * b / a;
//    }
//    else {
//        float q = (b > 0) ?
//        -0.5 * (b + sqrt(discr)) :
//        -0.5 * (b - sqrt(discr));
//        x0 = q / a;
//        x1 = c / q;
//    }
//    
//    return true;
//}

class Object {
public:
//    Object() : color(dis(gen), dis(gen), dis(gen)) {}
    Object(const Matrix44f &o2w) : objectToWorld(o2w), worldToObject(o2w.inverse()) {}
    virtual ~Object() {}
    virtual bool intersect(const Ray &, float &) const = 0;
    virtual void getSurfaceData(const Vec3f &, Vec3f &, Vec2f &) const = 0;
    Matrix44f objectToWorld, worldToObject;
    Vec3f albedo = 0.18;
};

#endif /* object_h */
