#ifndef GPU_RGB_CONV_H
#define GPU_RGB_CONV_H

#include "rgb_converter.h"
#include <iostream>
#include <stdlib.h>
#include <CL/cl.h>

class GpuRgbConv : public RgbConv
{

    public: 
    GpuRgbConv(std::unique_ptr<char[]> arg, unsigned int length, unsigned int start) : RgbConv(std::move(arg),length,start)
    {
        char* source_str = new char[MAX_SOURCE_SIZE];

        kp = fopen("kernel_rgb_conv.cl","r");
        if (!kp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }

        source_size = fread( source_str, 1, MAX_SOURCE_SIZE, kp);
        fclose(kp);

        cl_platform_id platform_id = NULL;
        cl_device_id device_id = NULL;   
        cl_uint ret_num_devices;
        cl_uint ret_num_platforms;
        cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
        ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, 
                &device_id, &ret_num_devices);
        checkRet(ret, 10);
        std::cout << device_id << std::endl;
        printf("nume_devices = %d , num_platforms = %d\n", ret_num_devices , ret_num_platforms);

        cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
        checkRet(ret, 0);

        cl_command_queue cmd_queue = clCreateCommandQueue(context, device_id, 0, &ret);
        checkRet(ret,1);

        cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, 
                length, NULL, &ret);
        cl_mem r_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, 
                length, NULL, &ret);
        checkRet(ret,2);

        ret = clEnqueueWriteBuffer(cmd_queue, a_mem_obj, CL_TRUE, 0,
                length, rgb_buffer.get(), 0, NULL, NULL);

        cl_program program = clCreateProgramWithSource(context, 1, 
        (const char**)&source_str, (const size_t*)&source_size, &ret);
        checkRet(ret,3);

        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        checkRet(ret , 4);

        cl_kernel kernel = clCreateKernel(program, "rgb_conv" , &ret);

        checkRet(ret,9);

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&a_mem_obj);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&r_mem_obj);

        checkRet(ret,5);

        
        size_t global_size = (length-138)/3;
        size_t local_size = 32;

        ret = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, &global_size, 
                    &local_size, 0, NULL, NULL);
        checkRet(ret,6);

        clFlush(cmd_queue);
        
        char* t = rgb_buffer.get();
        char* r = rgb_buffer.get();
        std::cout << "W" << std::endl;

        std::cout << "Reference : " << std::endl;
        std::cout << std::hex << (int)r[0] << std::endl;
        std::cout << std::hex << (int)r[1] << std::endl;
        std::cout << std::hex << (int)r[2] << std::endl;
        std::cout << std::endl;
        std::cout << std::hex << (int)r[3] << std::endl;
        std::cout << std::hex << (int)r[4] << std::endl;
        std::cout << std::hex << (int)r[5] << std::endl;

        ret = clEnqueueReadBuffer(cmd_queue, r_mem_obj, CL_TRUE, 0, 
                    length, t, 0, NULL, NULL);   
        clFlush(cmd_queue);
        std::cout<<"Output : "<<std::endl;                    
        checkRet(ret,7); 

        std::cout << std::hex << (int)t[0] << std::endl;
        std::cout << std::hex << (int)t[1] << std::endl;
        std::cout << std::hex << (int)t[2] << std::endl;
        std::cout << std::endl;
        std::cout << std::hex << (int)t[3] << std::endl;
        std::cout << std::hex << (int)t[4] << std::endl;
        std::cout << std::hex << (int)t[5] << std::endl;

    }

    std::unique_ptr<char[]> convert_to_rgb() override
    {

    }

    private:
        FILE* kp;
        const unsigned int MAX_SOURCE_SIZE = 0x100000;
        size_t source_size;

        void checkRet(cl_uint arg_ret, int arg_callNr)
        {
            if(arg_ret != 0)
            {
                    printf("Error at %d ret = %d \n",arg_callNr, arg_ret);
            }
        }

        bool checkResult(char* before , char* after, unsigned int length)
        {

        }

};


#endif