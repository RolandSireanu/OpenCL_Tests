#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "bmpFormat.h"
#include "cpu_rgb_conv.h"
#include "gpu_rgb_conv.h"
#include <CL/cl.h>


//Little endian on the disk 
//Interpreted in little endian format 

int main()
{

    

    GpuRgbConv gpu_conv("../images/input.bmp");
    auto start = std::chrono::high_resolution_clock::now();
    gpu_conv.convert_to_rgb("../outputImages/GpuOutput.bmp");
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "Duration : " << std::dec << duration.count() << " ms " << std::endl;
    

    CpuRgbConv cpu_conv("../images/input.bmp");   
    start = std::chrono::high_resolution_clock::now();
    cpu_conv.convert_to_rgb("../outputImages/CpuOutput.bmp");
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "Duration : " << std::dec << duration.count() << " ms " << std::endl;

    return 0;
}