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
#include "../utility/global.hpp"






bool PathTracer::trace(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects, IsectInfo &isect, RayType rayType)
{
    isect.hitObject = nullptr;

    std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
        float t = kInfinity;
        if ((*iter)->intersect(ray, t) && t < isect.tNear) {
            isect.hitObject = iter->get();
            isect.tNear = t;
        }
    }
    
    return (isect.hitObject != nullptr);
}

//, uint32_t depth
Vec3f PathTracer::castRay(const Ray &ray,
                          const std::vector<std::unique_ptr<Object>> &objects,
                          const std::vector<std::unique_ptr<Light>> &lights,
                          const Options &options,
                          uint32_t depth = 0)
{
    if (depth > options.maxDepth) return options.backgroundColor;
    Vec3f hitColor = 0;

    IsectInfo isect;
    
    if(trace(ray, objects, isect)){
        Vec3f Phit = ray.at(isect.tNear);
        Vec3f Nhit;
        Vec2f texCoordinates;

        isect.hitObject->getSurfaceData(Phit, Nhit, texCoordinates);
        
        switch (isect.hitObject->type) {
            case kDiffuse: // loop over all lights in the scene
            {
                for(uint32_t i = 0; i < lights.size(); i++){
                    Vec3f lightDir, lightIntensity;
                    IsectInfo isectShad;
                    lights[i]->illuminate(Phit, lightDir, lightIntensity, isectShad.tNear);
                    
//                    auto o = Phit + Nhit * options.bias;
//                    std::cout << o << std::endl;
//                    ray.ori = o;
                    
                    bool vis = trace(ray, objects, isectShad);  // ray here should add a bias !!!!!!!!!!!!!!!!!!!!
//                    std::cout << vis << std::endl;
                    hitColor += isect.hitObject->albedo / M_PI * lights[i]->intensity * lights[i]->color * std::max(0.f, Nhit.dotProduct(-ray.dir));
//                    float angle = radian(45);
//                    float s = texCoordinates.x * cos(angle) - texCoordinates.y * sin(angle);
//                    float t = texCoordinates.y * cos(angle) + texCoordinates.x * sin(angle);
//                    float scaleS = 20, scaleT = 20;
//                    float pattern = (modulo(s * scaleS) < 0.5);
//                    hitColor += vis * pattern * lightIntensity * std::max(0.f, Phit.dotProduct(-lightDir));
                }
                break;
            }
            case kReflection:
            {
                break;
            }
            case kReflectionAndRefraction:
            {
                break;
            }
            default:
                break;
        }
        
        // shade the hit point (a basic checker board pattern)
//        float scale = 4;
//        float pattern = (fmodf(textureCoordinates.x * scale, 1) > 0.5) ^ (fmodf(textureCoordinates.y * scale, 1) > 0.5);
//        hitColor = std::max(0.f, Nhit.dotProduct(-ray.dir)) * mix(hitObject->color, hitObject->color * 0.8, pattern);

    }
    else{
        hitColor = options.backgroundColor;
    }
    return hitColor;
}

void PathTracer::render(
        const Options &options,
        const std::vector<std::unique_ptr<Object>> &objects,
//        const std::vector<std::unique_ptr<Light>> &lights,
        const std::vector<std::unique_ptr<Light>> &lights)
    {
//        Vec3f *framebuffer = new Vec3f[options.width * options.height];
//        Vec3f *pix = framebuffer;
        std::unique_ptr<Vec3f []> framebuffer(new Vec3f[options.width * options.height]);
        Vec3f *pix = framebuffer.get();
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
                *(pix++) = castRay(ray, objects, lights, options);
            }
        }
        
        // Save result to a PPM image
        ImageIO image;
        std::string filename;
        image.save_ppm(options.width, options.height, framebuffer, filename);
        
        
//        delete [] framebuffer;
}
