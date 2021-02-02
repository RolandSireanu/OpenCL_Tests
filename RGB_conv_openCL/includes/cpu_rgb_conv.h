#ifndef CPU_RGB_CONV_H
#define CPU_RGB_CONV_H

#include "rgb_converter.h"
#include <fstream>

class CpuRgbConv : public RgbConv
{
    public:

        CpuRgbConv(std::string inputImage) 
            : RgbConv(inputImage)
        {

        }

        void convert_to_rgb(std::string outputImage) override 
        {
            char temp;
            unsigned int boundery = (rgb_length / 3);
            // std::cout << "boundery = " << boundery << std::endl;
            // std::cout << "rgb_length = " << rgb_length << std::endl;
            // std::cout << "rgb_start = " << rgb_start << std::endl;

            
            for(unsigned int i = 0; i < boundery; i++)
            {
                temp = rgb_buffer[rgb_start+(i*3)];
                rgb_buffer[rgb_start+(i*3)] = rgb_buffer[rgb_start+(i*3)+2];
                rgb_buffer[rgb_start+(i*3)+2] = temp;
            }

            
            std::ofstream outStream(outputImage, std::ios::out | std::ios::binary);
            outStream.write(rgb_buffer.get(), rgb_length);            
            outStream.close();
            
        }

};







#endif