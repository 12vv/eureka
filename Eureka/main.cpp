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
#include <sstream>
#include <chrono>

#include <unistd.h>  


//#include "./utility/global.hpp"
#include "./geometry/geometry.hpp"
#include "./geometry/ray.hpp"
#include "./utility/image_io.hpp"

#include "./core/path_tracer.hpp"
#include "./object/sphere.hpp"
#include "./object/trianglemesh.hpp"
#include "./light/distant.hpp"
#include "./light/point.hpp"
#include "./object/plane.hpp"
#include "./material/material.hpp"
#include "./material/mirror.hpp"
#include "./material/glossy.hpp"

template <> const Matrix44f Matrix44f::kIdentity = Matrix44f();

TriangleMesh* loadPolyMeshFromFile(const char *file, const Matrix44f &o2w, Material *m, const Vec3f &a)
{
//    char *buffer;
//    //也可以将buffer作为输出参数
//    if((buffer = getcwd(NULL, 0)) == NULL)
//    {
//        perror("getcwd error");
//    }
//    else
//    {
//        printf("%s\n", buffer);
//        free(buffer);
//    }
    
    std::ifstream ifs;
    try {
        ifs.open(file);
//        if (ifs.fail()) throw;
        if (ifs.fail()){
            std::cout << strerror(errno) << std::endl;
            throw;
        }
        std::stringstream ss;
        ss << ifs.rdbuf();
        uint32_t numFaces;
        ss >> numFaces;
        std::unique_ptr<uint32_t []> faceIndex(new uint32_t[numFaces]);
        uint32_t vertsIndexArraySize = 0;
        // reading face index array
        for (uint32_t i = 0; i < numFaces; ++i) {
            ss >> faceIndex[i];
            vertsIndexArraySize += faceIndex[i];
        }
        std::unique_ptr<uint32_t []> vertsIndex(new uint32_t[vertsIndexArraySize]);
        uint32_t vertsArraySize = 0;
        // reading vertex index array
        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> vertsIndex[i];
            if (vertsIndex[i] > vertsArraySize) vertsArraySize = vertsIndex[i];
        }
        vertsArraySize += 1;
        // reading vertices
        std::unique_ptr<Vec3f []> verts(new Vec3f[vertsArraySize]);
        for (uint32_t i = 0; i < vertsArraySize; ++i) {
            ss >> verts[i].x >> verts[i].y >> verts[i].z;
        }
        // reading normals
        std::unique_ptr<Vec3f []> normals(new Vec3f[vertsIndexArraySize]);
        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> normals[i].x >> normals[i].y >> normals[i].z;
        }
        // reading st coordinates
        std::unique_ptr<Vec2f []> st(new Vec2f[vertsIndexArraySize]);
        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> st[i].x >> st[i].y;
        }
        
        return new TriangleMesh(o2w, m, a, numFaces, faceIndex, vertsIndex, verts, normals, st);
    }
    catch (...) {
        ifs.close();
    }
    ifs.close();
    
    return nullptr;
}



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


Matrix44f camToWorld(const Vec3f& from, const Vec3f& to, const Vec3f& tmp = Vec3f(0, 1, 0))
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

    
    Matrix44f l2w2(0.95292, 0.289503, 0.0901785, 0, -0.0960954, 0.5704, -0.815727, 0, -0.287593, 0.768656, 0.571365, 0, 0, 0, 0, 1);
    Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0, -1.902827, -3.543982, -11.895445, 0, 5.459804, 10.568624, -4.02205, 0, 0, 0, 0, 1);
//    Matrix44f l2w;
    
    
///////////////////////
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Glossy(Material::kGlossy), Vec3f(0.5, 0.5, 0.5), 3, Vec3f(0, 10, 25))));
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Mirror(Material::kReflect), Vec3f(0, 0, 0), 4, Vec3f(20, 0, 20))));
    
//    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse), Vec3f(1, 1, 1), 4, Vec3f(5, 0, 0))));
    
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse), Vec3f(0.6, 0.3, 0.07), 3, Vec3f(18, -1, 25))));
    objects.push_back(std::unique_ptr<Object>(new Sphere(xform1, new Diffuse(Material::kDiffuse), Vec3f(0.9, 0.9, 1), 4, Vec3f(10, 0, 25))));

    // floor
    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Diffuse(Material::kDiffuse), Vec3f(1, 1, 1), Vec3f(0, -4, 0), Vec3f(0, 10, 0))));
    // back
    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Diffuse(Material::kDiffuse), Vec3f(0.5, 0.7, 1.0), Vec3f(0, 0, -100), Vec3f(0, 0, 1))));
    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Mirror(Material::kReflect), Vec3f(0.5, 0.5, 0.5), Vec3f(-5, 0, 0), Vec3f(1, 0, 0))));
    
//    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Diffuse(Material::kDiffuse), Vec3f(1, 1, 1), Vec3f(50, 0, 0), Vec3f(-1, 0, 0))));
    
//    objects.push_back(std::unique_ptr<Object>(new Plane(xform1, new Mirror(Material::kReflect), Vec3f(0.5, 0.5, 0.5), Vec3f(0, 50, 0), Vec3f(0, 1, 0))));
    

//    TriangleMesh *mesh = loadPolyMeshFromFile("./cylinder.geo", Matrix44f::kIdentity, new Glossy(Material::kGlossy));
//    if (mesh != nullptr) {
//        mesh->ior = 1.5;
//        objects.push_back(std::unique_ptr<Object>(mesh));
//    }

//    TriangleMesh *mesh4 = loadPolyMeshFromFile("./pen.geo", Matrix44f::kIdentity, new Diffuse(Material::kDiffuse));
//    if (mesh4 != nullptr) {
//        mesh4->albedo = 0.18;
//        mesh4->smoothShading = false;
//        objects.push_back(std::unique_ptr<Object>(mesh4));
//    }


//    TriangleMesh *mesh3 = loadPolyMeshFromFile("./cow.geo", Matrix44f::kIdentity, new Diffuse(Material::kDiffuse), Vec3f(0.6, 0.47, 0.07));
//    if (mesh3 != nullptr) {
//        mesh3->ior = 1.5;
//        objects.push_back(std::unique_ptr<Object>(mesh3));
//    }
    
    
    // setting up options
    Options options;
//    options.fov = 36.87;
    options.fov = 46.87;
    options.width = 1024;
    options.height = 747;
//    options.cameraToWorld = Matrix44f(0.999945, 0, 0.0104718, 0, 0.00104703, 0.994989, -0.0999803, 0, -0.0104193, 0.0999858, 0.994934, 0, -0.978596, 17.911879, 75.483369, 1);

    Matrix44f test = camToWorld(Vec3f(5, 15, 60), Vec3f(10, 10, 1));
    options.cameraToWorld = test;
    std::cout << options.cameraToWorld << std::endl;
    

    // finally, render
    lights.push_back(std::unique_ptr<Light>(new DistantLight(l2w, Vec3f(0, 10, -1), Vec3f(1, 1, 1), 2)));
//    lights.push_back(std::unique_ptr<Light>(new DistantLight(l2w, Vec3f(-5, 10, -1), Vec3f(1, 1, 1), 1)));
    lights.push_back(std::unique_ptr<Light>(new PointLight(l2w2, Vec3f(20, 10, 22), Vec3f(1, 1, 1), 5)));
//    lights.push_back(std::unique_ptr<Light>(new PointLight(l2w2, Vec3f(8, 10, 50), Vec3f(1, 1, 1), 1)));
    
    
    auto timeStart = std::chrono::high_resolution_clock::now();
    
    pt.render(options, objects, lights);
    
    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    std::cerr << passedTime << std::endl;
    
    return 0;
}
