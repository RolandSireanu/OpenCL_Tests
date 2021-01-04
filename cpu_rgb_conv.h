#ifndef CPU_RGB_CONV_H
#define CPU_RGB_CONV_H

#include "rgb_converter.h"

class CpuRgbConv : public RgbConv
{
    public:

        CpuRgbConv(std::unique_ptr<char[]> arg_buffer, unsigned int arg_length, unsigned int arg_start) 
            : RgbConv(std::move(arg_buffer), arg_length, arg_start)
        {

        }

        std::unique_ptr<char[]> convert_to_rgb() override 
        {
            char temp;
            unsigned int boundery = (rgb_length / 3);

            for(unsigned int i = 0; i < boundery; i++)
            {
                temp = rgb_buffer[rgb_start+(i*3)];
                rgb_buffer[rgb_start+(i*3)] = rgb_buffer[rgb_start+(i*3)+2];
                rgb_buffer[rgb_start+(i*3)+2] = temp;
            }

            return std::move(rgb_buffer);
            
        }

};







#endif