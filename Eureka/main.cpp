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

#include "./utility/global.hpp"
#include "./geometry/geometry.hpp"
#include "./geometry/ray.hpp"





class Light
{
public:
    Light() {}
};

class Object
{
public:
    Object() {}
    virtual ~Object() {}
};

// [comment]
// This function doesn't do much at the moment. It simply takes the ray direction
// and turn it into a color. Ray direction coordinates are un the range [-1,1].
// To normalized them, we just add 1 and divide the result by 2.
// [/comment]
Vec3f castRay(
              const Ray &ray,
              const std::vector<std::unique_ptr<Object>> &objects,
              const std::vector<std::unique_ptr<Light>> &lights,
              const Options &options,
              uint32_t depth)
{
    Vec3f hitColor = (ray.direction() + Vec3f(1)) * 0.5;
    return hitColor;
}

// [comment]
// The main render function. This where we iterate over all pixels in the image, generate
// primary rays and cast these rays into the scene. The content of the framebuffer is
// saved to a file.
// [/comment]
void render(
            const Options &options,
            const std::vector<std::unique_ptr<Object>> &objects,
            const std::vector<std::unique_ptr<Light>> &lights)
{
    Vec3f *framebuffer = new Vec3f[options.width * options.height];
    Vec3f *pix = framebuffer;
    float scale = tan(radian(options.fov * 0.5));
    float imageAspectRatio = options.width / (float)options.height;
    // [comment]
    // Don't forget to transform the ray origin (which is also the camera origin
    // by transforming the point with coordinates (0,0,0) to world-space using the
    // camera-to-world matrix.
    // [/comment]
    Vec3f orig;
    options.cameraToWorld.multVecMatrix(Vec3f(0), orig);
    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // [comment]
            // Generate primary ray direction. Compute the x and y position
            // of the ray in screen space. This gives a point on the image plane
            // at z=1. From there, we simply compute the direction by normalized
            // the resulting vec3f variable. This is similar to taking the vector
            // between the point on the image plane and the camera origin, which
            // in camera space is (0,0,0):
            //
            // ray.dir = normalize(Vec3f(x,y,-1) - Vec3f(0));
            // [/comment]
#ifdef MAYA_STYLE
            float x = (2 * (i + 0.5) / (float)options.width - 1) * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale * 1 / imageAspectRatio;
#else
            
            float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
#endif
            // [comment]
            // Don't forget to transform the ray direction using the camera-to-world matrix.
            // [/comment]
//            Vec3f dir;
            Ray ray(Vec3f(0,0,0), Vec3f(1,1,1));
            options.cameraToWorld.multVecMatrix(Vec3f(x, y, -1), ray.dir);
            ray.dir.normalize();
            *(pix++) = castRay(ray, objects, lights, options, 0);
        }
    }
    
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << options.width << " " << options.height << "\n255\n";
    for (uint32_t i = 0; i < options.height * options.width; ++i) {
        char r = (char)(255 * clamp(0, 1, framebuffer[i].x));
        char g = (char)(255 * clamp(0, 1, framebuffer[i].y));
        char b = (char)(255 * clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }
    
    ofs.close();
    
    delete [] framebuffer;
}

// [comment]
// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image widht and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
// [/comment]
int main(int argc, char **argv)
{
    // creating the scene (adding objects and lights)
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    
    // setting up options
    Options options;
    options.width = 640;
    options.height = 480;
    options.fov = 90;
    options.cameraToWorld = Matrix44f(0.945519, 0, -0.325569, 0, -0.179534, 0.834209, -0.521403, 0, 0.271593, 0.551447, 0.78876, 0, 4.208271, 8.374532, 17.932925, 1);
    
    // finally, render
    render(options, objects, lights);
    
    return 0;
}
