#include "OCLKernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Util.h"

static inline int FloorPow2(int n)
{
    int exp;
    frexp((float)n, &exp);
    return 1 << (exp - 1);
}

OCLKernel::OCLKernel(const char* kernel_path, const char* kernel_name, int device_type)
{
	m_deviceType = device_type;
	m_kernelPath = kernel_path;
	m_kernelName = kernel_name;
	
	fprintf(stdout, "Compiling %s for main function %s...\n", m_kernelPath, m_kernelName);
	
	cl_int err; // for error code
	
	// connect to a compute device
	err = clGetPlatformIDs(1, &m_clpid, NULL); // get 1 platform id and stash it in m_clpid
	if(err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to find platform!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}
	
	// get a device of the appropriate type
	err = clGetDeviceIDs(m_clpid, m_deviceType, 1, &m_devid, NULL); 
	if(err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create a device group!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}
	
	// create a compute context
	m_ctx = clCreateContext(0, 1, &m_devid, NULL, NULL, &err);
	if(!m_ctx || err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create a compute context!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}
	
	// create a command queue
	m_cmd_q = clCreateCommandQueue(m_ctx, m_devid, 0, &err);
	if(!m_cmd_q || err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create a command queue!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}
	
	m_kernelSource = readFile(m_kernelPath);
	if(m_kernelSource == "")
	{
		fprintf(stderr, "Error: Failed to read kernel file %s!\n", m_kernelPath);
		exit(EXIT_FAILURE);
	}
	
	// create the m_program from the source buffer
    const char *srcptr = m_kernelSource.c_str();
	m_program = clCreateProgramWithSource(m_ctx, 1, &srcptr, NULL, &err);
	if(!m_program || err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create program!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}
	
	// compile the m_program
	err = clBuildProgram(m_program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to compile program %s\n", m_kernelPath);
		print_cl_err(err);
		size_t len;
		char buffer[2048];
		clGetProgramBuildInfo(m_program, m_devid, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		fprintf(stderr, "%s\n", buffer);
		exit(EXIT_FAILURE);
	}
	
	// create the m_kernel in the m_program
	m_kernel = clCreateKernel(m_program, m_kernelName, &err);
	if(!m_kernel || err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create kernel!\n");
		print_cl_err(err);
		exit(EXIT_FAILURE);
	}

}

OCLKernel::~OCLKernel()
{
	clReleaseProgram(m_program);
	clReleaseKernel(m_kernel);
	clReleaseCommandQueue(m_cmd_q);
	clReleaseContext(m_ctx);
}

int OCLKernel::run(OCLArgumentArray array, int global_width, int global_height)
{
    int buffer_count = array.bufferArguments().size();
    int arg_count = array.arguments().size();
    std::vector<OCLArgument> buffers = array.bufferArguments();
    std::vector<OCLArgument> args = array.arguments();

	cl_mem cl_buffers[buffer_count];
	cl_int err;
	
	for(int i = 0; i < buffer_count; i++)
	{
		if(buffers[i].buffer_type == WRITE)
		{
			cl_buffers[i] = clCreateBuffer(m_ctx, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buffers[i].byte_size, buffers[i].data, &err);
			if(!cl_buffers[i] || err != CL_SUCCESS)
			{
				fprintf(stderr, "Error: Failed to allocate device memory for buffer %d (WRITE)!\n", i);
				print_cl_err(err);
				return EXIT_FAILURE;
			}
		}
		else if(buffers[i].buffer_type == READ)
		{
			cl_buffers[i] = clCreateBuffer(m_ctx, CL_MEM_READ_ONLY, buffers[i].byte_size, buffers[i].data, &err);
			if(!cl_buffers[i] || err != CL_SUCCESS)
			{
				fprintf(stderr, "Error: Failed to allocate device memory for buffer %d (READ)!\n", i);
				print_cl_err(err);
				return EXIT_FAILURE;
			}
		}
		else if(buffers[i].buffer_type == READ_WRITE)
		{
			cl_buffers[i] = clCreateBuffer(m_ctx, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, buffers[i].byte_size, buffers[i].data, &err);
			if(!cl_buffers[i] || err != CL_SUCCESS)
			{
				fprintf(stderr, "Error: Failed to allocate device memory for buffer %d (READ_WRITE)!\n", i);
				print_cl_err(err);
				return EXIT_FAILURE;
			}
		}
	}
	
	for(int i = 0; i < arg_count; i++)
	{
		if(args[i].is_buffer)
		{
			cl_mem buf = cl_buffers[args[i].buffer_index];
			err = clSetKernelArg(m_kernel, i, sizeof(cl_mem), &buf);
		}
		else
		{
			err = clSetKernelArg(m_kernel, i, args[i].byte_size, args[i].data);
		}
			
		if(err != CL_SUCCESS)
		{
			fprintf(stderr, "Error: Failed to set kernel argument %d!\n", i);
			print_cl_err(err);
			return EXIT_FAILURE;
		}
	}

	size_t global[2] = {global_width, global_height};
	size_t local[2];
	
	// get the maximum work group size for executing the kernel on the device
	size_t max = 1;
	err = clGetKernelWorkGroupInfo(m_kernel, m_devid, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &max, NULL);
	if(err != CL_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to retrieve kernel work group info!\n");
		print_cl_err(err);
		return EXIT_FAILURE;
	}
	
	local[0] = (max > 1) ? FloorPow2(max) : max;  // use nearest power of two (less than max)
	if(local[0] == max)
		local[0] /= 2;
	
	if(local[0] > global[0])
		local[0] = global[0];
		
	local[1] = 1;
	
	// execute the kernel over the buffer using the maximum number of work_group items for the device
	err = clEnqueueNDRangeKernel(m_cmd_q, m_kernel, 2, NULL, global, local, 0, NULL, NULL);
	if(err != CL_SUCCESS) 
	{
		fprintf(stderr, "%d %d\n", global[0]*global[1], local[0]*local[1]);
		fprintf(stderr, "Error: Failed to execute kernel!\n");
		print_cl_err(err);
		return EXIT_FAILURE;
	}
	
	// wait for the computation to finish
	clFinish(m_cmd_q);
	
	for(int i = 0; i < buffer_count; i++)
	{
		if(buffers[i].buffer_type == READ || buffers[i].buffer_type == READ_WRITE)
		{
			// get the results
			err = clEnqueueReadBuffer(m_cmd_q, cl_buffers[i], CL_TRUE, 0, buffers[i].byte_size, buffers[i].data, 0, NULL, NULL);
			if(err != CL_SUCCESS)
			{
				fprintf(stderr, "Error: Failed to read output buffer %d!\n", i);
				print_cl_err(err);
				return EXIT_FAILURE;
			}
		}
	}
	
	for(int i = 0; i < buffer_count; i++)
	{
		clReleaseMemObject(cl_buffers[i]);
	}
	
	return EXIT_SUCCESS;
}

//---------------------------------------
// OCLArgumentArray
//---------------------------------------

OCLArgumentArray::OCLArgumentArray()
{
}

void OCLArgumentArray::appendArgument(void *data, size_t size)
{
    OCLArgument a;
    a.data = data;
    a.byte_size = size;
    a.is_buffer = false;
    m_arguments.push_back(a);
}

void OCLArgumentArray::appendBufferArgument(void *data, size_t size)
{
    OCLArgument a;
    a.data = data;
    a.byte_size = size;
    a.is_buffer = true;
    a.buffer_index = m_bufferArguments.size();
    a.buffer_type = READ_WRITE;
    m_arguments.push_back(a);
    m_bufferArguments.push_back(a);
}














