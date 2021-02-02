#include <CL/cl2.hpp>
#include <CL/cl.h>
#include "cl.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


using namespace std::chrono;

unsigned getDeviceList(std::vector<cl::Device>& devices)
{
  cl_int err;

  // Get list of platforms
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  // Enumerate devices
  for (int i = 0; i < platforms.size(); i++)
  {
    cl_uint num = 0;
    std::vector<cl::Device> plat_devices;
    platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &plat_devices);
    devices.insert(devices.end(), plat_devices.begin(), plat_devices.end());
  }

  return devices.size();
}

void getDeviceName(cl::Device& device, std::string& name)
{
  cl_device_info info = CL_DEVICE_NAME;

  // Special case for AMD
#ifdef CL_DEVICE_BOARD_NAME_AMD
  device.getInfo(CL_DEVICE_VENDOR, &name);
  if (strstr(name.c_str(), "Advanced Micro Devices"))
    info = CL_DEVICE_BOARD_NAME_AMD;
#endif

  device.getInfo(info, &name);
}

inline std::string loadProgram(std::string input)
{
    std::ifstream stream(input.c_str());
    if (!stream.is_open()) {
        std::cout << "Cannot open file: " << input << std::endl;
        exit(1);
    }

     return std::string(
        std::istreambuf_iterator<char>(stream),
        (std::istreambuf_iterator<char>()));
}


void initMatrix(std::vector<float>& arg_vector , float arg_value, unsigned int arg_order)
{
    for(int i=0; i<arg_order; i++)
        for(int j=0; j<arg_order; j++)
        {
            arg_vector[i*arg_order+j] = arg_value;
        }
}

int main()
{
    const unsigned int ORDER = 1024;
    const unsigned int SIZE = ORDER * ORDER;
    unsigned int deviceIdx;
    
    std::vector<float> A(SIZE);
    std::vector<float> B(SIZE);
    std::vector<float> C(SIZE);

    cl::Buffer bufferA, bufferB, bufferC;
    std::vector<cl::Device> platform_devices;
    cl::Device deviceHW;
    std::vector<cl::Device> devices;
    getDeviceList(devices);
    if(devices.size() > 1)
    {
        for(auto it = devices.begin(); it != devices.end(); it++)
        {
            std::string tempDeviceName;        
            it->getInfo(CL_DEVICE_NAME, &tempDeviceName);
            std::cout << "Device " << tempDeviceName << std::endl;
        }
    }
    else
    {
        deviceIdx = 0;
    }

    cl::Device device = devices[deviceIdx];
    std::string temp; 
    device.getInfo(CL_DEVICE_NAME, &temp);
    std::cout << "Kernel will be run on : " << temp << std::endl; 

    std::vector<cl::Device> usedDevices;
    usedDevices.push_back(device);
    cl::Context context(usedDevices);
    cl::CommandQueue queue(context, device);

    initMatrix(A, 1.0f, ORDER);
    initMatrix(B, 2.0f, ORDER);
    initMatrix(C, 1.00f, ORDER);

    bufferA = cl::Buffer(context, A.begin(), A.end(), true);
    bufferB = cl::Buffer(context, B.begin(), B.end(), true);
    bufferC = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float)*SIZE);
    // ===========================================================
    cl_int err;
    // cl::Program program(context, loadProgram("./MatrixMul.cl"), true, &err);
    // std::cout<<"Build program result : " << err << std::endl;
    // cl::compatibility::make_kernel<int, cl::Buffer, cl::Buffer, cl::Buffer> mmul(program, "mmul");

    // cl::NDRange global(ORDER,ORDER);
    // cl::NDRange local (2*2);

     unsigned int totalDuration=0;
    // for(int i=0; i<200; i++)
    // {
    //     auto start_time = high_resolution_clock::now();
    //     mmul(cl::EnqueueArgs(queue, global, local), ORDER, bufferA, bufferB, bufferC);
    //     queue.finish();        
    //     auto stop = high_resolution_clock::now(); 
    //     auto duration = duration_cast<microseconds>(stop - start_time);
    //     totalDuration += duration.count();        
    // }
    // std::cout << "Runtime : " << totalDuration <<std::endl;
    // cl::copy(queue, bufferC, C.begin(), C.end());
    // std::cout << C[0] << std::endl; 
    // std::cout << C[1000] << std::endl;
    // ============================================================

    cl::Program fastProgram(context, loadProgram("./FastMatrixMul.cl"), true, &err);
    std::cout<<"Build program result : " << err << std::endl;
    cl::compatibility::make_kernel<int, cl::Buffer, cl::Buffer, cl::Buffer> fastMmul(fastProgram, "fastMmul");

    cl::NDRange globalDim(ORDER);
    

    totalDuration=0;
    for(int i=0; i<1; i++)
    {
        auto start_time = high_resolution_clock::now();
        fastMmul(cl::EnqueueArgs(queue, globalDim), ORDER, bufferA, bufferB, bufferC);
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start_time);
        totalDuration += duration.count();        
    }
    std::cout << "Runtime : " << totalDuration <<std::endl;


    queue.finish();
    cl::copy(queue, bufferC, C.begin(), C.end());
    std::cout << C[0] << std::endl; 
    std::cout << C[1000] << std::endl;


    return 0;
}