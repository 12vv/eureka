//
//  path_tracer.cpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/28.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#include "path_tracer.hpp"
//#include "../utility/global.hpp"
#include "../utility/image_io.hpp"


//const float PI = 3.141592653589793238;
//const float PI_INV = 1.0 / PI;
//const float DOU_PI = 2.0 * PI;
//const float DOU_PI_INV = 1.0 / DOU_PI;
const float kInfinity = std::numeric_limits<double>::max();
//const float kEpsilon = 1e-4;
//const float E = 2.718281828459045;



bool PathTracer::trace(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects, float &tNear, const Object *&hitObject)
{
    tNear = kInfinity;

    std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        float t = kInfinity;
        if ((*iter)->intersect(ray, t) && t < tNear) {
            hitObject = iter->get();
            tNear = t;
        }
    }
    
    return (hitObject != nullptr);
}

//, uint32_t depth
Vec3f PathTracer::castRay(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects){
    Vec3f hitColor = 0;
    const Object *hitObject = nullptr; // a pointer to the hit object
    float t; // the intersection distrance
    
    
    if(trace(ray, objects, t, hitObject)){
        Vec3f Phit = ray.at(t);
        Vec3f Nhit;
        Vec2f tex;

        hitObject->getSurfaceData(Phit, Nhit, tex);
        // shade the hit point (a basic checker board pattern)
        float scale = 4;
        float pattern = (fmodf(tex.x * scale, 1) > 0.5) ^ (fmodf(tex.y * scale, 1) > 0.5);
        hitColor = std::max(0.f, Nhit.dotProduct(-ray.dir)) * mix(hitObject->color, hitObject->color * 0.8, pattern);
    }
    return hitColor;
}

void PathTracer::render(
        const Options &options,
        const std::vector<std::unique_ptr<Object>> &objects)
    {
        Vec3f *framebuffer = new Vec3f[options.width * options.height];
        Vec3f *pix = framebuffer;
        float scale = tan(radian(options.fov * 0.5));
        float imageAspectRatio = options.width / (float)options.height;

        Ray ray;
//        Vec3f orig;
//        options.cameraToWorld.multPtMatrix(Vec3f(0), orig);
        options.cameraToWorld.multPtMatrix(Vec3f(0), ray.ori);
        
        for (uint32_t j = 0; j < options.height; ++j) {
            for (uint32_t i = 0; i < options.width; ++i) {

#ifdef MAYA_STYLE
                float x = (2 * (i + 0.5) / (float)options.width - 1) * scale;
                float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale * 1 / imageAspectRatio;
#else
                
                float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
#endif

                
                // transform to world space
                options.cameraToWorld.multVecMatrix(Vec3f(x, y, -1), ray.dir);
                ray.dir.normalize();
                *(pix++) = castRay(ray, objects);
            }
        }
        
        // Save result to a PPM image
        ImageIO image;
        std::string filename;
        image.save_ppm(options.width, options.height, framebuffer, filename);
        
        
        delete [] framebuffer;
}
