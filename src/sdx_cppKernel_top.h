#include  "ap_int.h"
#ifndef SDX_CPPKERNEL_TOP_H_
#define SDX_CPPKERNEL_TOP_H_ 

#ifndef NUMBER_OF_COMPUTE_UNITS
#define NUMBER_OF_COMPUTE_UNITS 1 
#endif

//#define data_t double
#define data_t ap_uint<512>

#define LOCAL_DATA_SIZE 1024*1024*16
#define LOCAL_DATA_SIZE_IN_MB (LOCAL_DATA_SIZE*sizeof(data_t))/(1024*1024)
//#define LOCAL_DATA_SIZE 8*8
//#define GLOBAL_DATA_SIZE LOCAL_DATA_SIZE*4
#define GLOBAL_DATA_SIZE LOCAL_DATA_SIZE*1
#define GLOBAL_DATA_SIZE_IN_MB (GLOBAL_DATA_SIZE*sizeof(data_t))/(1024*1024)

#define MAX_ITERATION GLOBAL_DATA_SIZE/(NUMBER_OF_COMPUTE_UNITS* LOCAL_DATA_SIZE) 

void simple_kernel (data_t *a_in, data_t *results);

#ifdef XOCC_CPP_KERNEL 
extern "C" {
#endif
void sdx_cppKernel_top(data_t a_in[GLOBAL_DATA_SIZE], data_t y_out[GLOBAL_DATA_SIZE]);
#ifdef XOCC_CPP_KERNEL 
}
#endif

#endif
