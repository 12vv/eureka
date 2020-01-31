//
//  image_io.cpp
//  Eureka
//
//  Created by Jaythan Yip on 2020/1/27.
//  Copyright © 2020 Jaythan. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <sstream>

#include "global.hpp"
#include "image_io.hpp"


std::string ImageIO::generate_file_name()
{
    time_t t;
    struct tm *tt;
    time(&t);
    tt = localtime(&t);
    std::ostringstream file;
    file << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
    return file.str();
}


bool ImageIO::save_ppm(int width, int height, const std::unique_ptr<Vec3f []> &framebuffer, std::string &file)
{
    file = generate_file_name();
    file += ".ppm";
    std::cerr << "save to: " << file << std::endl;
    
    std::ofstream ofs(file, std::ios::out | std::ios::binary);
    if (!ofs.is_open()) { std::cerr << "ppm格式图片保存失败 :(\n"; return false; }
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        char r = (char)(255 * clamp(0, 1, framebuffer[i].x));
        char g = (char)(255 * clamp(0, 1, framebuffer[i].y));
        char b = (char)(255 * clamp(0, 1, framebuffer[i].z));
        
        ofs << r << g << b;
    }
    
    ofs.close();
    return true;
}
