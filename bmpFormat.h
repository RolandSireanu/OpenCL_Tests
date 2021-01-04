
#ifndef BMP_H
#define BMP_H

#include <fstream>
#include <memory>

namespace BMP 
{

    const unsigned int PIXEL_SIZE = 28;         // 28 .. 29
    const unsigned int WIDTH = 18;               // 18 .. 21
    const unsigned int HEIGHT = 22;               // 22 .. 25
    const unsigned int DATA_OFFSET = 10;        //10 .. 13
    const unsigned int SIZE = 2;                //2 .. 5

    class BmpFormat
    {        

        public:    

            BmpFormat(std::string arg_bmp_path) : stream(arg_bmp_path, std::ios::in | std::ios::binary)
            {
                unsigned int size = ReadSize();

                binImage = std::make_unique<char[]>(size);
                stream.seekg(0, std::ios::beg);
                stream.read(binImage.get(), size);                
            }

            std::unique_ptr<char[]> getByteArray()
            {
                return std::move(binImage);
            }


            short ReadPixelSize()
            {
                short temp;
                stream.seekg(PIXEL_SIZE, std::ios::beg);
                stream.read((char*)&temp , 2);

                return temp;

            }


            unsigned int ReadWidth()
            {
                unsigned int temp;
                stream.seekg(WIDTH, std::ios::beg);
                stream.read((char*)&temp, 4);

                return temp;
            }


            unsigned int ReadHeight()
            {
                unsigned int temp;
                stream.seekg(HEIGHT, std::ios::beg);
                stream.read((char*)&temp, 4);

                return temp;
            }


            unsigned int ReadDataOffset()
            {
                unsigned int temp;
                stream.seekg(DATA_OFFSET, std::ios::beg);
                stream.read((char*)&temp, 4);

                return temp;
            }

            unsigned int ReadSize()
            {
                unsigned int temp;
                stream.seekg(SIZE, std::ios::beg);
                stream.read((char*)&temp, 4);

                return temp;
            }

            ~BmpFormat()
            {
                stream.close();
            }

        private:
            unsigned int totalFileSize;
            std::ifstream stream;
            std::unique_ptr<char[]> binImage;

    };

};




#endif