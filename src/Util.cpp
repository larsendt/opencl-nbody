#include "Util.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_SZ 4096

std::string readFile(const char* filename)
{
	FILE* f = fopen(filename, "r");
    std::string text;

	if(f)
	{
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		char *str = new char[size+1];
		fread(str, size, 1, f);
	    str[size-1] = '\0';
		fclose(f);
        text = str;
        delete[] str;        
	}
    
    return text;
}

void print_cl_err(int err_code)
{
	switch(err_code)
	{
		case 0:
			fprintf(stderr, "Error code: 0 CL_SUCCESS\n");
			break;
		case -1:
			fprintf(stderr, "Error code: -1 CL_DEVICE_NOT_FOUND\n");
			break;
		case -2:
			fprintf(stderr, "Error code: -2 CL_DEVICE_NOT_AVAILABLE\n");
			break;
		case -3:
			fprintf(stderr, "Error code: -3 CL_COMPILER_NOT_AVAILABLE\n");
			break;
		case -4:
			fprintf(stderr, "Error code: -4 CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
			break;
		case -5:
			fprintf(stderr, "Error code: -5 CL_OUT_OF_RESOURCES\n");
			break;
		case -6:
			fprintf(stderr, "Error code: -6 CL_OUT_OF_HOST_MEMORY\n");
			break;
		case -7:
			fprintf(stderr, "Error code: -7 CL_PROFILING_INFO_NOT_AVAILABLE\n");
			break;
		case -8:
			fprintf(stderr, "Error code: -8 CL_MEM_COPY_OVERLAP\n");
			break;
		case -9:
			fprintf(stderr, "Error code: -9 CL_IMAGE_FORMAT_MISMATCH\n");
			break;
		case -10:
			fprintf(stderr, "Error code: -10 CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
			break;
		case -11:
			fprintf(stderr, "Error code: -11 CL_BUILD_PROGRAM_FAILURE\n");
			break;
		case -12:
			fprintf(stderr, "Error code: -12 CL_MAP_FAILURE\n");
			break;
		case -30:
			fprintf(stderr, "Error code: -30 CL_INVALID_VALUE\n");
			break;
		case -31:
			fprintf(stderr, "Error code: -31 CL_INVALID_DEVICE_TYPE\n");
			break;
		case -32:
			fprintf(stderr, "Error code: -32 CL_INVALID_PLATFORM\n");
			break;
		case -33:
			fprintf(stderr, "Error code: -33 CL_INVALID_DEVICE\n");
			break;
		case -34:
			fprintf(stderr, "Error code: -34 CL_INVALID_CONTEXT\n");
			break;
		case -35:
			fprintf(stderr, "Error code: -35 CL_INVALID_QUEUE_PROPERTIES\n");
			break;
		case -36:
			fprintf(stderr, "Error code: -36 CL_INVALID_COMMAND_QUEUE\n");
			break;
		case -37:
			fprintf(stderr, "Error code: -37 CL_INVALID_HOST_PTR\n");
			break;
		case -38:
			fprintf(stderr, "Error code: -38 CL_INVALID_MEM_OBJECT\n");
			break;
		case -39:
			fprintf(stderr, "Error code: -39 CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
			break;
		case -40:
			fprintf(stderr, "Error code: -40 CL_INVALID_IMAGE_SIZE\n");
			break;
		case -41:
			fprintf(stderr, "Error code: -41 CL_INVALID_SAMPLER\n");
			break;
		case -42:
			fprintf(stderr, "Error code: -42 CL_INVALID_BINARY\n");
			break;
		case -43:
			fprintf(stderr, "Error code: -43 CL_INVALID_BUILD_OPTIONS\n");
			break;
		case -44:
			fprintf(stderr, "Error code: -44 CL_INVALID_PROGRAM\n");
			break;
		case -45:
			fprintf(stderr, "Error code: -45 CL_INVALID_PROGRAM_EXECUTABLE\n");
			break;
		case -46:
			fprintf(stderr, "Error code: -46 CL_INVALID_KERNEL_NAME\n");
			break;
		case -47:
			fprintf(stderr, "Error code: -47 CL_INVALID_KERNEL_DEFINITION\n");
			break;
		case -48:
			fprintf(stderr, "Error code: -48 CL_INVALID_KERNEL\n");
			break;
		case -49:
			fprintf(stderr, "Error code: -49 CL_INVALID_ARG_INDEX\n");
			break;
		case -50:
			fprintf(stderr, "Error code: -50 CL_INVALID_ARG_VALUE\n");
			break;
		case -51:
			fprintf(stderr, "Error code: -51 CL_INVALID_ARG_SIZE\n");
			break;
		case -52:
			fprintf(stderr, "Error code: -52 CL_INVALID_KERNEL_ARGS\n");
			break;
		case -53:
			fprintf(stderr, "Error code: -53 CL_INVALID_WORK_DIMENSION\n");
			break;
		case -54:
			fprintf(stderr, "Error code: -54 CL_INVALID_WORK_GROUP_SIZE\n");
			break;
		case -55:
			fprintf(stderr, "Error code: -55 CL_INVALID_WORK_ITEM_SIZE\n");
			break;
		case -56:
			fprintf(stderr, "Error code: -56 CL_INVALID_GLOBAL_OFFSET\n");
			break;
		case -57:
			fprintf(stderr, "Error code: -57 CL_INVALID_EVENT_WAIT_LIST\n");
			break;
		case -58:
			fprintf(stderr, "Error code: -58 CL_INVALID_EVENT\n");
			break;
		case -59:
			fprintf(stderr, "Error code: -59 CL_INVALID_OPERATION\n");
			break;
		case -60:
			fprintf(stderr, "Error code: -60 CL_INVALID_GL_OBJECT\n");
			break;
		case -61:
			fprintf(stderr, "Error code: -61 CL_INVALID_BUFFER_SIZE\n");
			break;
		case -62:
			fprintf(stderr, "Error code: -62 CL_INVALID_MIP_LEVEL\n");
			break;
		case -63:
			fprintf(stderr, "Error code: -63 CL_INVALID_GLOBAL_WORK_SIZE\n");
			break;
		default:
			fprintf(stderr, "Unknown error code: %d\n", err_code);
			break;
	}
}
