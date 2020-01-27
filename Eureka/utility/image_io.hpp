//
//  image_io.hpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/27.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef image_io_hpp
#define image_io_hpp

#include <string.h>

class ImageIO {
public:
    ImageIO(){}
    
    static bool save_ppm(int width, int height, const Vec3f *framebuffer, std::string &file);
    
    static std::string generate_file_name();
};

#endif /* image_io_hpp */
