#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "bmpFormat.h"
#include "cpu_rgb_conv.h"
#include "gpu_rgb_conv.h"


// void printByte(unsigned char byteOffset, std::ifstream& arg_stream)
// {
//     unsigned char temp;
//     arg_stream.seekg(byteOffset, std::ios::beg);
//     arg_stream.read((char*)&temp,1);
//     std::cout << std::hex << (int)temp << std::endl;
// }

// typedef struct 
// {
//     unsigned char ch_1;
//     unsigned char ch_2;
//     unsigned char ch_3;
// }TPixel;



//Little endian on the disk 
//Interpreted in little endian format 

int main()
{
    std::string image = "input.bmp";
    std::string outImage = "out.bmp";
    BMP::BmpFormat bmp(image);
    
    std::ifstream stream(image, std::ios::in|std::ios::binary);
    std::streampos pos;
    stream.seekg(0, std::ios::end);
    pos = stream.tellg();
    std::cout << "Size = " << pos << std::endl;
    stream.close();


    std::unique_ptr<char[]> bin = bmp.getByteArray();
    unsigned int start_data = bmp.ReadDataOffset();
    unsigned int total_length = bmp.ReadSize();
    std::cout << "Size = " << total_length << std::endl;

    GpuRgbConv cpu_conv(std::move(bin), total_length, start_data);

    // auto start = std::chrono::high_resolution_clock::now();
    // std::unique_ptr<char[]> out = cpu_conv.convert_to_rgb();
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    // std::cout << "Duration : " << duration.count() << " ms " << std::endl;

    // std::ofstream outStream(outImage, std::ios::out | std::ios::binary);
    // outStream.write(out.get(), total_length);
    // outStream.close();

    //CpuRgbConv cpu_conv(bin.get(), bmp.Read());


    // std::string outImage = "out.bmp";
    // std::ifstream inputStream(image, std::ios::in | std::ios::binary);
    // std::ofstream outStream(outImage, std::ios::out | std::ios::binary);
    // std::streampos pos;
    // char* binImage;
    // unsigned int binImageSize = 0;

    // if(inputStream.is_open())
    // {

    //     inputStream.seekg(0, std::ios::end);
    //     pos = inputStream.tellg();
    //     std::cout << " File size = " << pos << " Bytes" << std::endl;
    //     binImageSize = pos;
    //     inputStream.seekg(0, std::ios::beg);
    // }
    // else
    // {
    //     std::cout << "Error at file opening ! " << std::endl;
    // }

    // printByte(0, inputStream);
    // printByte(1, inputStream);

    // std :: cout <<  BMP::ReadPixelSize(inputStream) << std::endl;
    // std :: cout <<  BMP::ReadWidth(inputStream) << std::endl;
    // std :: cout <<  BMP::ReadDataOffset(inputStream) << std::endl;
    // std :: cout <<  BMP::ReadSize(inputStream) << std::endl;

    // binImage = new char[binImageSize];    

    // inputStream.seekg(0, std::ios::beg);
    // inputStream.read(binImage, binImageSize);        
    // inputStream.close();

    // TPixel *ptr = (TPixel*)(binImage+138);
    
    
    //ch_3 = R
    //ch_1 = B
    //ch_2 = G

    // for(int i = 0; i < (1920*1280); i++)
    // {
    //     ptr[i].ch_3 = 0;    
    //     ptr[i].ch_1 = 0;   
    // }

    // outStream.write(binImage, binImageSize);
    // outStream.close();



    return 0;
}