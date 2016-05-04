#include  "ap_int.h"
#ifndef SDX_CPPKERNEL_TOP_H_
#define SDX_CPPKERNEL_TOP_H_ 

//#define data_t double 
#define data_t ap_uint<512>

#define SDX_BUS_WIDTH 512
#define SDX_BUS_WIDTH_BYTES SDX_BUS_WIDTH/8
#define SDX_CU_LOCAL_SIZE 64 

#define sdx_data_t ap_uint<SDX_BUS_WIDTH>

#define MAX_ITERATION 1 
//#define LOCAL_DATA_SIZE SDX_CU_LOCAL_SIZE*32
#define LOCAL_DATA_SIZE SDX_CU_LOCAL_SIZE*1024*256
#define LOCAL_DATA_SIZE_IN_MB (LOCAL_DATA_SIZE*sizeof(data_t))/(1024*1024)
#define GLOBAL_DATA_SIZE LOCAL_DATA_SIZE*MAX_ITERATION
#define GLOBAL_DATA_SIZE_IN_MB (GLOBAL_DATA_SIZE*sizeof(data_t))/(1024*1024)


const long int HLS_AXI_SIM_DEPTH=(long int)LOCAL_DATA_SIZE;

void simple_kernel (data_t *a_in, data_t *results);

#ifdef XOCC_CPP_KERNEL 
extern "C" {
#endif
void sdx_cppKernel_top(data_t *a_in, data_t *y_out);
#ifdef XOCC_CPP_KERNEL 
}
#endif

#endif
