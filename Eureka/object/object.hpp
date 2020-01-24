//
//  object.h
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/23.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef object_h
#define object_h

#include "../utility/global.hpp"

class Object {
public:
    Object() : color(dis(gen), dis(gen), dis(gen)) {}
    virtual ~Object() {}
    virtual bool intersect(const Vec3f &, const Vec3f &, float &) const = 0;
};

#endif /* object_h */
