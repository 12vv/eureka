//
//  mateiral.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/2/1.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../light/light.hpp"

class Material {
public:
    enum Type { kDiffuse, kReflect, kGlossy };
    Material(Type type) {}
    virtual ~Material() {}
//    Vec3f sample(const std::vector<std::unique_ptr<Light>> &lights, const Vec3f &Phit, const Vec3f &Nhit, const Options &options, const std::vector<std::unique_ptr<Object>> &objects);
    
//    Vec3f brdf() const
//    {
//        Vec3f color;
//        switch (type) {
//            case kDiffuse:
//                sampleDiffuse();
//                break;
////            case kReflect:
////                sampleReflect();
////                break;
////            case kRefract:
////                sampleRefract();
////                break;
////            case kPhong:
////                samplePhong(isect, out, in, pdf);
////                break;
////            case kGlossy:
////                sampleGlossy(isect, out, in, pdf);
////                break;
////            case kRetro:
////                sampleRetro(isect, out, in, pdf);
////                break;
////            case kHalton:
////                sampleHalton(isect, out, in, pdf);
////                break;
//            default:
//                assert(0);
//        }
//        return color;
//    }
    
    Type type;
    const char *name = "d";
};

//void Material::sampleDiffuse(){
//
//}

#endif /* mateiral_h */
