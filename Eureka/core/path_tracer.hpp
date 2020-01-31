//
//  path_tracer.hpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/28.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef path_tracer_hpp
#define path_tracer_hpp

#include <stdio.h>

#include "../geometry/geometry.hpp"
#include "../geometry/ray.hpp"
#include "../object/object.hpp"
#include "../light/distant.hpp"

enum RayType { kPrimaryRay, kShadowRay };

struct IsectInfo
{
    const Object *hitObject = nullptr;
    float tNear = kInfinity;
    Vec2f uv;
    uint32_t index = 0;
};



class PathTracer {
public:
//    PathTracer(){}
    
    bool trace(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects, IsectInfo &isect, RayType rayType = kPrimaryRay);
    Vec3f castRay(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects, const std::vector<std::unique_ptr<Light>> &lights, const Options &options, uint32_t depth);
    void render(const Options &options,
                const std::vector<std::unique_ptr<Object>> &objects,
                const std::vector<std::unique_ptr<Light>> &lights);
    
    
};

#endif /* path_tracer_hpp */
