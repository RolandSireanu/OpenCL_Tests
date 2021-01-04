#ifndef RGB_CONVERTER_H
#define RGB_CONVERTER_H

#include <memory>

class RgbConv
{

    public:

        RgbConv(std::unique_ptr<char[]> arg, unsigned int length, unsigned int start) : 
            rgb_buffer(std::move(arg)), 
            rgb_length(length),
            rgb_start(start)
        {}

        virtual std::unique_ptr<char[]> convert_to_rgb() = 0;    
        virtual ~RgbConv() {}
    
    protected:
        std::unique_ptr<char[]> rgb_buffer;
        const unsigned int rgb_length;
        const unsigned int rgb_start;

};


#endif