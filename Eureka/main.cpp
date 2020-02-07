//
//  main.cpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/13.
//  Copyright © 2020 Jaythan. All rights reserved.
//

//#include <iostream>
//#include "geometry.h"

//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}


#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>
#include <utility>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <random>

//#include "./utility/global.hpp"
#include "./geometry/geometry.hpp"
#include "./geometry/ray.hpp"
#include "./utility/image_io.hpp"

#include "./core/path_tracer.hpp"
#include "./object/sphere.hpp"
#include "./light/distant.hpp"
#include "./light/point.hpp"
#include "./object/plane.hpp"
#include "./material/material.hpp"
#include "./material/mirror.hpp"
#include "./material/glossy.hpp"


//static const float kInfinity = std::numeric_limits<float>::max();

//static const Vec3f kDefaultBackgroundColor = Vec3f(0.235294, 0.67451, 0.843137);




std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

//struct Options
//{
//    uint32_t width;
//    uint32_t height;
//    float fov;
//    Matrix44f cameraToWorld;
//    Vec3f backgroundColor = kDefaultBackgroundColor;
//};


Matrix44f lookAt(const Vec3f& from, const Vec3f& to, const Vec3f& tmp = Vec3f(0, 1, 0))
{
    Vec3f forward = (from - to).normalize();
    Vec3f t = tmp;
    Vec3f right = t.normalize().crossProduct(forward);
    Vec3f up = forward.crossProduct(right);
    
    Matrix44f camToWorld;
    
    camToWorld[0][0] = right.x;
    camToWorld[0][1] = right.y;
    camToWorld[0][2] = right.z;
    camToWorld[1][0] = up.x;
    camToWorld[1][1] = up.y;
    camToWorld[1][2] = up.z;
    camToWorld[2][0] = forward.x;
    camToWorld[2][1] = forward.y;
    camToWorld[2][2] = forward.z;
    
    camToWorld[3][0] = from.x;
    camToWorld[3][1] = from.y;
    camToWorld[3][2] = from.z;
    
    return camToWorld;
}

int main(int argc, char **argv)
{
    
    PathTracer pt;
    // creating the scene (adding objects and lights)
    std::vector<std::unique_ptr<Object>> objects;
    // lights
    std::vector<std::unique_ptr<Light>> lights;
    
    Matrix44f xform1;
    xform1[3][0] = -1.2;
    xform1[3][1] = 6;
    xform1[3][2] = -3;
//    Sphere *sph1 = new Sphere(xform1, 5);

    
//Matrix44f l2w(0.95292, 0.289503, 0.0901785, 0, -0.0960954, 0.5704, -0.815727, 0, -0.287593, 0.768656, 0.571365, 0, 0, 0, 0, 1);
    Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0, -1.902827, -3.543982, -11.895445, 0, 5.459804, 10.568624, -4.02205, 0, 0, 0, 0, 1);
//    Matrix44f l2w;
    
    
    // generate a scene made of random spheres
    uint32_t numSpheres = 1;
    gen.seed(0);
    for (uint32_t i = 0; i < numSpheres; ++i) {
//        Vec3f randPos((0.5 - dis(gen)) * 10, (0.5 - dis(gen)) * 10, (0.5 + dis(gen) * 10));
//        float randRadius = (0.5 + dis(gen) * 0.5);
//        objects.push_back(std::unique_ptr<Object>(new Sphere(randPos, randRadius)));
//        objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse, Vec3f(0, 0, 1)), 5, Vec3f(6, 0, 0))));
    }
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Mirror(Material::kReflect, Vec3f(1, 0, 0)), 4, Vec3f(-3, 0, 0))));
    
    
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Glossy(Material::kGlossy, Vec3f(0, 1, 0)), 5, Vec3f(12, 15, 35))));
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Mirror(Material::kReflect, Vec3f(1, 0, 0)), 4, Vec3f(30, 0, 0))));
    
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse, Vec3f(0, 1, 0)), 4, Vec3f(15, 0, 20))));
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse, Vec3f(0, 1, 0)), 4, Vec3f(10, 0, 50))));
    
    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Diffuse(Material::kDiffuse, Vec3f(1, 0, 0)), Vec3f(0, -4, 0), Vec3f(0, 10, 0))));
    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Mirror(Material::kReflect, Vec3f(1, 0, 0)), Vec3f(-10, 0, 0), Vec3f(1, 0, 0))));
//    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Mirror(Material::kReflect, Vec3f(1, 0, 0)), Vec3f(0, 25, 0), Vec3f(0, -10, 0))));
//    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Mirror(Material::kReflect, Vec3f(1, 0, 0)), Vec3f(65, 0, 0), Vec3f(-1, 0, 0))));
    
    // setting up options
    Options options;
//    options.fov = 36.87;
    options.fov = 96.87;
    options.width = 1024;
    options.height = 747;
//    options.cameraToWorld = Matrix44f(0.999945, 0, 0.0104718, 0, 0.00104703, 0.994989, -0.0999803, 0, -0.0104193, 0.0999858, 0.994934, 0, -0.978596, 17.911879, 75.483369, 1);

    Matrix44f test = lookAt(Vec3f(10, 30, 80), Vec3f(15, 0, 20));
    options.cameraToWorld = test;
    std::cout << options.cameraToWorld << std::endl;
    
//    const std::unique_ptr<DistantLight> light = std::unique_ptr<DistantLight>(new DistantLight(l2w, 1, 1));
    
//    lights.push_back(std::unique_ptr<Light>(new DistantLight(l2w, 1, 1)));
    // finally, render
    lights.push_back(std::unique_ptr<Light>(new DistantLight(l2w, Vec3f(0, 10, -1), Vec3f(0.5, 0.5, 1), 1)));
    lights.push_back(std::unique_ptr<Light>(new PointLight(l2w, Vec3f(0, 1, 0), Vec3f(1, 1, 1), 1)));
//    lights.push_back(std::unique_ptr<Light>(new PointLight(l2w, Vec3f(5, 1, 0), Vec3f(0.1, 0.5, 1), 1)));
    
    
    
    pt.render(options, objects, lights);
    
    return 0;
}
