#ifndef CPU_RGB_CONV_H
#define CPU_RGB_CONV_H

#include "rgb_converter.h"

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

            for(unsigned int i = 0; i < boundery; i++)
            {
                temp = rgb_buffer[rgb_start+(i*3)];
                rgb_buffer[rgb_start+(i*3)] = rgb_buffer[rgb_start+(i*3)+2];
                rgb_buffer[rgb_start+(i*3)+2] = temp;
            }

            std::ofstream outStream(outputImage, std::ios::out | std::ios::binary);
            outStream.write(output, rgb_length);
            outStream.close();
            
        }

};







#endif