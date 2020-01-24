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
#include "../geometry/geometry.hpp"

const float PI = 3.141592653589793238;
const float PI_INV = 1.0 / PI;
const float DOU_PI = 2.0 * PI;
const float DOU_PI_INV = 1.0 / DOU_PI;
const float kInfinity = std::numeric_limits<double>::max();
const float kEpsilon = 1e-4;
const float E = 2.718281828459045;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

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

struct Options
{
    uint32_t width;
    uint32_t height;
    float fov;
    Matrix44f cameraToWorld;
};



#endif /* global_h */
