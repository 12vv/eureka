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

Vec3f refract(const Vec3f &I, const Vec3f &N, const float &ior)
{
    float cosi = clamp(-1, 1, I.dotProduct(N));
    float etai = 1, etat = ior;
    Vec3f n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void fresnel(const Vec3f &I, const Vec3f &N, const float &ior, float &kr)
{
    float cosi = clamp(-1, 1, I.dotProduct(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
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
        if (objects[k]->intersect(ray, tNear, index, uv) && tNear < isect.tNear) {
            if (rayType == kShadowRay && (dynamic_cast< Glossy* >( objects[k]->type )) != nullptr) continue;
            isect.hitObject = objects[k].get();
            isect.tNear = tNear;
            isect.index = index;
            isect.uv = uv;
        }

    }
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

        isect.hitObject->getSurfaceData(Phit, ray.dir, isect.index, isect.uv, Nhit, texCoordinates);
        
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
//                hitColor += vis * isect.hitObject->albedo / M_PI * lights[i]->intensity * lights[i]->color * std::max(0.f, Nhit.dotProduct(-lightDir));
                 hitColor += vis * isect.hitObject->albedo * lights[i]->intensity * std::max(0.f, Nhit.dotProduct(-lightDir));
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

        
        if((dynamic_cast< Glossy* >(type)) != nullptr){
            Vec3f refractionColor = 0;
            // compute fresnel
            float kr;
            isect.hitObject->ior = 1.4;  // self-add !!!!!!!!!
            Vec3f dir = ray.direction();
            fresnel(dir, Nhit, isect.hitObject->ior, kr);
            bool outside = dir.dotProduct(Nhit) < 0;
            Vec3f bias = options.bias * Nhit;
            // compute refraction if it is not a case of total internal reflection
            if (kr < 1) {
                Vec3f refractionDirection = refract(dir, Nhit, isect.hitObject->ior).normalize();
                Vec3f refractionRayOrig = outside ? Phit - bias : Phit + bias;
                Ray tmp(refractionRayOrig, refractionDirection);
                refractionColor = castRay(tmp, objects, lights, options, depth + 1);
            }
            
            Vec3f reflectionDirection = reflect(dir, Nhit).normalize();
            Vec3f reflectionRayOrig = outside ? Phit + bias : Phit - bias;
            Ray tmp(reflectionDirection, reflectionRayOrig);
            Vec3f reflectionColor = castRay(tmp, objects, lights, options, depth + 1);
            
            // mix the two
            hitColor += reflectionColor * kr + refractionColor * (1 - kr);
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
