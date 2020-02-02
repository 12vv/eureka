//
//  image_io.hpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/27.
//  Copyright © 2020 Jaythan. All rights reserved.
//

#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <string.h>

class ImageIO {
public:
    ImageIO(){}
    
    static bool save_ppm(int width, int height, const std::unique_ptr<Vec3f []> &, std::string &file);
    
    static std::string generate_file_name();
};

#endif /* image_io_hpp */
