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


int main()
{
    unsigned int deviceIdx;
    const unsigned int LOCAL_D = 256;
    const unsigned int STEPS = 1024*16;
    const unsigned int ITTERS = 128*128;
    const unsigned int NWORK_GROUPS = (STEPS/LOCAL_D);

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
    // ===========================================================
    cl_int err;

    unsigned int totalDuration=0;

    cl::Program bin(context, loadProgram("./Integration.cl"), true, &err);
    std::cout<<"Build program result : " << err << std::endl;    
    
    cl::compatibility::make_kernel<int, float, cl::LocalSpaceArg, cl::Buffer> pi_computation(bin, "pi_computation");
    std::cout << " A" << std::endl;

    cl::NDRange globalDim(STEPS);
    cl::NDRange localDim(LOCAL_D);
    float step = 1.0 / (STEPS*ITTERS);
    std::cout << "step = " << step << std::endl;
    
    cl::Buffer partialSums = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * NWORK_GROUPS);    
    std::vector<float> partialSumsLocal(NWORK_GROUPS);

    pi_computation(cl::EnqueueArgs(queue, globalDim, localDim), ITTERS, step,
                    cl::Local(sizeof(float) * LOCAL_D),
                    partialSums);

    //cl::copy(queue, d_partial_sums, h_psum.begin(), h_psum.end());
    for(int i=0; i<NWORK_GROUPS; i++)
    {
        partialSumsLocal[i] = 0.0f;
    }
    cl::copy(queue,partialSums, partialSumsLocal.begin(), partialSumsLocal.end());
    float result = 0.0f;

    for(int i=0; i<NWORK_GROUPS; i++)
    {
        result += partialSumsLocal[i];
    }

    std::cout << "Pi =" << result*step << std::endl;
 
    queue.finish();

    std::cout << " C" << std::endl;

    return 0;
}