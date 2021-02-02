#ifndef RGB_CONVERTER_H
#define RGB_CONVERTER_H

#include <memory>

class RgbConv
{

    public:

        RgbConv(std::string inImage)
        {            
            BMP::BmpFormat bmpImage(inImage);
            rgb_buffer = std::move(bmpImage.getByteArray());
            rgb_start = bmpImage.ReadDataOffset();
            rgb_length = bmpImage.ReadSize();            
            bmpImage.CloseStream();
        }

        virtual void convert_to_rgb(std::string outputImage) = 0;    
        virtual ~RgbConv() 
        {

        }
    
    protected:
        std::unique_ptr<char[]> rgb_buffer;        
        unsigned int rgb_length;
        unsigned int rgb_start;

};


#endif