//
//  geometry.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/21.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef geometry_h
#define geometry_h

#include <iostream>
#include <math.h>
#include <stdlib.h>


template <typename T>
class Vec3 {
public:
    Vec3(): x(0), y(0), z(0) {}
    Vec3(T xx): x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}
    
    Vec3 operator + (const Vec3 &v) const
    {return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator - (const Vec3 &v) const
    { return Vec3(x - v.x, y - v.y, z - v.z); }
    T dotProduct(const Vec3<T> &v) const
    { return x * v.x + y * v.y + z * v.z; }
    T crossProduct(const Vec3<T> &v) const
    { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    T norm() const
    { return x * x + y * y + z * z; }
    T length() const
    { return sqrt(norm()); }
    
    //[comment]
    // The next two operators are sometimes called access operators or
    // accessors. The Vec coordinates can be accessed that way v[0], v[1], v[2],
    // rather than using the more traditional form v.x, v.y, v.z.
    //[/comment]
    const T& operator [] (uint8_t i) const { return (&x)[i]; }
    T& operator [] (uint8_t i) { return (&x)[i]; }
    
    Vec3& normalize(){
        T n = norm();
        if(n > 0){
            T factor = 1 / sqrt(n);
            x *= factor; y *= factor; z *= factor;
        }
        return *this;
    }
    
    friend std::ostream& operator << (std::ostream &os, Vec3<T> &v)
    { return os << '(' << v.x << ' ' << v.y << ' ' << v.z << ')'; }
    friend std::istream& operator >> (std::istream &is, Vec3<T> &v)
    { return is >> v.x >> v.y >> v.z; }
    
    
    T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;


template <typename T>
class Vec2 {
public:
    Vec2(): x(0), y(0) {}
    Vec2(T xx): x(xx), y(xx) {}
    Vec2(T xx, T yy): x(xx), y(yy) {}
    
    Vec2 operator + (const Vec2 &v) const
    {return Vec2(x + v.x, y + v.y); }
    Vec2 operator - (const Vec2 &v) const
    { return Vec2(x - v.x, y - v.y); }
    T dotProduct(const Vec2<T> &v) const
    { return x * v.x + y * v.y; }
    float crossProduct(const Vec2<T> &v) const
    { return (x * v.y - y * v.x); } // return the z-component
    T norm() const
    { return x * x + y * y; }
    T length() const
    { return sqrt(norm()); }
    

    const T& operator [] (uint8_t i) const { return (&x)[i]; }
    T& operator [] (uint8_t i) { return (&x)[i]; }
    
    Vec2& normalize(){
        T n = norm();
        if(n > 0){
            T factor = 1 / sqrt(n);
            x *= factor; y *= factor;
        }
        return *this;
    }
    
    friend std::ostream& operator << (std::ostream &os, Vec2<T> &v)
    { return os << '(' << v.x << ' ' << v.y << ')'; }
    friend std::istream& operator >> (std::istream &is, Vec2<T> &v)
    { return is >> v.x >> v.y; }
    
    
    T x, y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;


#endif /* geometry_h */
