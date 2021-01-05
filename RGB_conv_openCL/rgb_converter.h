#ifndef RGB_CONVERTER_H
#define RGB_CONVERTER_H

#include <memory>

class RgbConv
{

    public:

        RgbConv(std::string inImage)
        {            
            BMP::BmpFormat bmpImage(inImage);
            rgb_buffer = bmpImage.getByteArray();
            rgb_start = bmpImage.ReadDataOffset();
            rgb_length = bmpImage.ReadSize();
            output = new char[rgb_length];
        }

        virtual void convert_to_rgb(std::string outputImage) = 0;    
        virtual ~RgbConv() 
        {
            delete output;
        }
    
    protected:
        std::unique_ptr<char[]> rgb_buffer;
        char* output;
        unsigned int rgb_length;
        unsigned int rgb_start;

};


#endif