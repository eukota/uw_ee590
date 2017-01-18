#pragma once
#include "CL/cl.h"
#include <string>

/* Convenient container for all OpenCL specific objects used in the sample
*
* It consists of two parts:
*   - regular OpenCL objects which are used in almost each normal OpenCL applications
*   - several OpenCL objects that are specific for this particular sample
*
* You collect all these objects in one structure for utility purposes
* only, there is no OpenCL specific here: just to avoid global variables
* and make passing all these arguments in functions easier.
*/
struct ocl_args_d_t
{
	ocl_args_d_t();
	~ocl_args_d_t();

	// Regular OpenCL objects:
	cl_context       context;           // hold the context handler
	cl_device_id     device;            // hold the selected device handler
	cl_command_queue commandQueue;      // hold the commands-queue handler
	cl_program       program;           // hold the program handler
	cl_kernel        kernel;            // hold the kernel handler
	float            platformVersion;   // hold the OpenCL platform version (default 1.2)
	float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
	float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)

	int SetupOpenCL(cl_device_type deviceType);
	int CreateAndBuildProgram(const std::string& filename);
	int GetPlatformAndDeviceVersion(cl_platform_id platformId);
	cl_uint ExecuteKernel(size_t *globalWorkSize, cl_uint workSizeCount);
};

int CreateReadBufferArg_Float(cl_context *context, cl_mem* mem, cl_float* input);
int CreateReadBufferArg_FloatArray(cl_context *context, cl_mem* mem, cl_float* input, cl_uint arrayWidth, cl_uint arrayHeight);
int CreateWriteBufferArg_FloatArray(cl_context *context, cl_mem* mem, cl_float* output, cl_uint arrayWidth, cl_uint arrayHeight);
cl_uint SetKernelArgument(cl_kernel* kernel, cl_mem* mem, unsigned int argNum);
cl_uint MapHostBufferToLocal(cl_command_queue* commandQueue, cl_mem* hostMem, cl_uint width, cl_uint height, cl_float** localMem);
cl_uint UnmapHostBufferFromLocal(cl_command_queue* commandQueue, cl_mem* hostMem, cl_float* localMem);