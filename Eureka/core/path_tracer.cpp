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
#include "../material/diffuse.hpp"
#include "../material/glossy.hpp"
#include "../material/mirror.hpp"

Vec3f reflect(const Vec3f &I, const Vec3f &N)
{
    return I - 2 * I.dotProduct(N) * N;
}





bool PathTracer::trace(const Ray &ray,
                       const std::vector<std::unique_ptr<Object>> &objects,
                       IsectInfo &isect,
                       RayType rayType)
{
    isect.hitObject = nullptr;
    
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNear = kInfinity;
        uint32_t index = 0;
        Vec2f uv;
        if (objects[k]->intersect(ray, tNear) && tNear < isect.tNear) {
            if (rayType == kShadowRay && (dynamic_cast< Glossy* >( objects[k]->type )) != nullptr) continue;
            isect.hitObject = objects[k].get();
            isect.tNear = tNear;
            isect.index = index;
            isect.uv = uv;
        }
    }

//    std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();
//    for (; iter != objects.end(); ++iter) {
//        float t = kInfinity;
//        if ((*iter)->intersect(ray, t) && t < isect.tNear) {
//            isect.hitObject = iter->get();
//            isect.tNear = t;
//        }
//    }
    
    return (isect.hitObject != nullptr);
}


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
        
        Material *type = isect.hitObject->type;
        
        if((dynamic_cast< Diffuse* >(type)) != nullptr){

            for(uint32_t i = 0; i < lights.size(); i++){
                Vec3f lightDir, lightIntensity;
                IsectInfo isectShad;
                lights[i]->illuminate(Phit, lightDir, lightIntensity, isectShad.tNear);
                
                Ray tmp(Phit + Nhit * options.bias, -lightDir);
                bool vis = !trace(tmp, objects, isectShad, kShadowRay);
                //                    bool vis = trace(ray, objects, isectShad);  // ray here should add a bias !!!!!!!!!!!!!!!!!!!!
                //                    std::cout << vis << std::endl;
                hitColor += vis * isect.hitObject->albedo / M_PI * lights[i]->intensity * lights[i]->color * std::max(0.f, Nhit.dotProduct(-ray.dir));
//                 hitColor = isect.hitObject->albedo / M_PI * lights[i]->intensity * lights[i]->color * std::max(0.f, Nhit.dotProduct(-ray.dir));

                                    float angle = radian(45);
                                    float s = texCoordinates.x * cos(angle) - texCoordinates.y * sin(angle);
                                    float t = texCoordinates.y * cos(angle) + texCoordinates.x * sin(angle);
                                    float scaleS = 20, scaleT = 20;
//                                    float pattern = (modulo(s * scaleS) < 0.5);
                                    float pattern = (modulo(s * scaleS) < 0.5) ^ (modulo(t * scaleT) < 0.5);
//                                    hitColor += vis * pattern * lightIntensity * std::max(0.f, Phit.dotProduct(-lightDir));
            }
        }
        

        if((dynamic_cast< Mirror* >(type)) != nullptr){
            Vec3f R = reflect(ray.dir, Nhit);
            R.normalize();
            Ray tmp(Phit + Nhit * options.bias, R);
            hitColor += 0.8 * castRay(tmp, objects, lights, options, depth + 1);
        }


    }
    else{
        hitColor = options.backgroundColor;
    }
    return hitColor;
}

void PathTracer::render(
        const Options &options,
        const std::vector<std::unique_ptr<Object>> &objects,
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
