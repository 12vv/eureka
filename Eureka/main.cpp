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


//static const float kInfinity = std::numeric_limits<float>::max();
static const float kEpsilon = 1e-8;
//static const Vec3f kDefaultBackgroundColor = Vec3f(0.235294, 0.67451, 0.843137);

enum MaterialType { kDiffuse, kReflection, kReflectionAndRefraction };


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



int main(int argc, char **argv)
{
    
    PathTracer pt;
    // creating the scene (adding objects and lights)
    std::vector<std::unique_ptr<Object>> objects;
    // lights
//    std::vector<std::unique_ptr<Light>> lights;
    
    Matrix44f xform1;
    xform1[3][0] = -1.2;
    xform1[3][1] = 6;
    xform1[3][2] = -3;
//    Sphere *sph1 = new Sphere(xform1, 5);
    
Matrix44f l2w(0.95292, 0.289503, 0.0901785, 0, -0.0960954, 0.5704, -0.815727, 0, -0.287593, 0.768656, 0.571365, 0, 0, 0, 0, 1); 
    
    // generate a scene made of random spheres
    uint32_t numSpheres = 1;
    gen.seed(0);
    for (uint32_t i = 0; i < numSpheres; ++i) {
//        Vec3f randPos((0.5 - dis(gen)) * 10, (0.5 - dis(gen)) * 10, (0.5 + dis(gen) * 10));
//        float randRadius = (0.5 + dis(gen) * 0.5);
//        objects.push_back(std::unique_ptr<Object>(new Sphere(randPos, randRadius)));
        objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, 5)));
    }
    
    
    // setting up options
    Options options;
    options.fov = 36.87;
    options.width = 1024;
    options.height = 747;
    options.cameraToWorld = Matrix44f(0.999945, 0, 0.0104718, 0, 0.00104703, 0.994989, -0.0999803, 0, -0.0104193, 0.0999858, 0.994934, 0, -0.978596, 17.911879, 75.483369, 1);
    


    const std::unique_ptr<DistantLight> light = std::unique_ptr<DistantLight>(new DistantLight(l2w, 1, 1));
    
//    lights.push_back(std::unique_ptr<Light>(new DistantLight(l2w, 1, 1)));
    // finally, render
    pt.render(options, objects, light);
    
    return 0;
}
