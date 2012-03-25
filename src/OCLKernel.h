#ifndef OCL_KERNEL_H
#define OCL_KERNEL_H

#include <CL/cl.h>

enum BufferType {
	READ,
	WRITE,
	READ_WRITE
};

struct OCLArgument {
	void* data;
	size_t byte_size;
	bool is_buffer;
	int buffer_index; // the index within either the read_buffers or write_buffers arrays
	BufferType buffer_type;
};

class OCLKernel 
{
	public:
		OCLKernel(const char* kernel_path, const char* kernel_name, int device_type = CL_DEVICE_TYPE_GPU);
		~OCLKernel();
		int run(int arg_count, OCLArgument* args, 
		          int buffer_count, OCLArgument* buffers,
		          int global_width, int global_height);
		
	private:
		int m_deviceType;
		const char* m_kernelPath;
		const char* m_kernelSource;
		const char* m_kernelName;
		
		cl_platform_id m_clpid;   // OpenCL platform
		cl_device_id m_devid;     // compute device id
		cl_context m_ctx;         // compute context
		cl_command_queue m_cmd_q;  // compute command queue
		cl_program m_program;     // compute program
		cl_kernel m_kernel;       // compute kernel
};

	

#endif
