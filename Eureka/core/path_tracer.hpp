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

class PathTracer {
public:
//    PathTracer(){}
    
    bool trace(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects, float &tNear, const Object *&hitObject);
    Vec3f castRay(const Ray &ray, const std::vector<std::unique_ptr<Object>> &objects); //, uint32_t depth
    void render(const Options &options,
                const std::vector<std::unique_ptr<Object>> &objects);
    
    
};

#endif /* path_tracer_hpp */
