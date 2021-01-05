#ifndef GPU_RGB_CONV_H
#define GPU_RGB_CONV_H

#include "rgb_converter.h"
#include "bmpFormat.h"
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <CL/cl.h>
#include <stdio.h>
#include <string.h>

class GpuRgbConv : public RgbConv
{

    public: 
    GpuRgbConv(std::string inputImage) : RgbConv(inputImage)
    {

    }

    void convert_to_rgb(std::string outputImage) override
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
                rgb_length, NULL, &ret);        
        checkRet(ret,2);

        ret = clEnqueueWriteBuffer(cmd_queue, a_mem_obj, CL_TRUE, 0,
                rgb_length, rgb_buffer.get(), 0, NULL, NULL);

        cl_program program = clCreateProgramWithSource(context, 1, 
        (const char**)&source_str, (const size_t*)&source_size, &ret);
        checkRet(ret,3);

        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        checkRet(ret , 4);

        cl_kernel kernel = clCreateKernel(program, "rgb_conv" , &ret);
        checkRet(ret,9);

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&a_mem_obj);        
        checkRet(ret,5);

        
        size_t global_size = (rgb_length-138)/3;
        size_t local_size = 32;

        ret = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, &global_size, 
                    &local_size, 0, NULL, NULL);
        checkRet(ret,6);

        clFlush(cmd_queue);
        
        char* before = new char[rgb_length];
        memcpy(before, rgb_buffer.get(), rgb_length);        

        ret = clEnqueueReadBuffer(cmd_queue, a_mem_obj, CL_TRUE, 0, 
                    rgb_length, output, 0, NULL, NULL);   
        clFlush(cmd_queue); 
        checkRet(ret,7);

        checkResult(before, output);
        checkHeader(before, output);

        std::ofstream outStream(outputImage, std::ios::out | std::ios::binary);
        outStream.write(output, rgb_length);
        outStream.close();

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

        bool checkResult(char* before , char* after)
        {
                std::cout << "rgb_start = " << rgb_start << std::endl;
                std::cout << "rgb_length = " << rgb_length << std::endl;
                
                for(unsigned int i = 0; i < rgb_length; i+=3)
                {
                        if(before[rgb_start+i] != after[rgb_start+(i+2)])
                        {       
                                std::cout << "Result failed at index : " << i << std::endl;
                                std::cout << (int)before[rgb_start+i] << " vs " << (int)after[rgb_start+(i+2)] << std::endl;
                                std::cout << rgb_start+i << " and " << rgb_start+(i+2) << std::endl;
                                exit(0);
                        }
                }
        }

        bool checkHeader(char* before, char* after)
        {
                for(unsigned int i = 0; i < rgb_start; i++)
                {
                        if(before[i] != after[i])
                        {
                                std::cout << "Header not the same at byte : " << i << std::endl;
                                std::cout << "Before : " << std::ios::hex << (int)before[i] << std::endl;
                                std::cout << "After : " << std::ios::hex << (int)after[i] << std::endl;
                                exit(0);
                        }
                }
        }

};


#endif