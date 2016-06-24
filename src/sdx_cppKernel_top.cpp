#include <string.h>

#include "sdx_cppKernel_top.h"
#include <stdio.h>

#include "hls_stream.h"

void sdx_cppKernel_top(sdx_data_t a_in[LOCAL_DATA_SIZE], sdx_data_t y_out[LOCAL_DATA_SIZE]) {
#pragma HLS INTERFACE m_axi port=a_in offset=slave depth=HLS_AXI_SIM_DEPTH latency=100 bundle=gmem num_read_outstanding=64 num_write_outstanding=64 max_read_burst_length=16 max_write_burst_length=16
#pragma HLS INTERFACE m_axi port=y_out offset=slave depth=HLS_AXI_SIM_DEPTH latency=100 bundle=gmem num_read_outstanding=64 num_write_outstanding=64 max_read_burst_length=16 max_write_burst_length=16
#pragma HLS INTERFACE s_axilite port=a_in bundle=control
#pragma HLS INTERFACE s_axilite port=y_out bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    sdx_data_t bufa_val[SDX_CU_LOCAL_SIZE];
    sdx_data_t bufy_val[SDX_CU_LOCAL_SIZE];


    sdx_cppKernel_top_local_data_loop:for (int i = 0; i < (LOCAL_DATA_SIZE/SDX_CU_LOCAL_SIZE); i++) {
    #pragma HLS PIPELINE II=1
        memcpy(bufa_val, a_in, SDX_BUS_WIDTH_BYTES*SDX_CU_LOCAL_SIZE);
        a_in += SDX_CU_LOCAL_SIZE;
        for (int itn_num = 0 ; itn_num < SDX_CU_LOCAL_SIZE; itn_num++) {
            simple_kernel (&bufa_val[itn_num], &bufy_val[itn_num]);
        }
        memcpy(y_out, bufy_val, SDX_BUS_WIDTH_BYTES*SDX_CU_LOCAL_SIZE);
        y_out += SDX_CU_LOCAL_SIZE;
    }
    return;
}

