//
//  global.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/24.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef global_h
#define global_h

#include <cmath>
#include <random>
#include "../geometry/geometry.hpp"

//const float PI = 3.141592653589793238;
//const float PI_INV = 1.0 / PI;
//const float DOU_PI = 2.0 * PI;
//const float DOU_PI_INV = 1.0 / DOU_PI;
const float kInfinity = std::numeric_limits<double>::max();
static const float kEpsilon = 1e-8;
//const float E = 2.718281828459045;
static const Vec3f kDefaultBackgroundColor = Vec3f(0.235294, 0.67451, 0.843137);
enum MaterialType { kDiffuse, kReflection, kReflectionAndRefraction };

inline
float radian(const float &degree){
    return degree * M_PI / 180;
}

inline
float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }


inline
Vec3f mix(const Vec3f &a, const Vec3f& b, const float &mixValue)
{ return a * (1 - mixValue) + b * mixValue; }

inline float modulo(const float &f)
{
    return f - std::floor(f);
}

struct Options
{
    uint32_t width = 640;
    uint32_t height = 480;
    float fov = 90;
    Vec3f backgroundColor = kDefaultBackgroundColor;
    Matrix44f cameraToWorld;
    float bias = 0.0001;
    uint32_t maxDepth = 5;
}; 



#endif /* global_h */
