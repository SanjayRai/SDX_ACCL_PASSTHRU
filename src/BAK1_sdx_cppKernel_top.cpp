#include <string.h>

#include "sdx_cppKernel_top.h"
#include <stdio.h>

void sdx_cppKernel_top(data_t *a_in, data_t *y_out)
{
#pragma HLS INTERFACE m_axi port=a_in offset=slave bundle=gmem depth=1024*64*4
#pragma HLS INTERFACE m_axi port=y_out offset=slave bundle=gmem depth=1024*64*4
#pragma HLS INTERFACE s_axilite port=a_in bundle=control
#pragma HLS INTERFACE s_axilite port=y_out bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control


  data_t bufa[LOCAL_DATA_SIZE];
  data_t *a_in_ptr;
  data_t *bufa_ptr;
  data_t bufy[LOCAL_DATA_SIZE];
  data_t *y_out_ptr;
  data_t *bufy_ptr;


  a_in_ptr = a_in;
  y_out_ptr = y_out;


    sdx_cppKernel_top_local_global_data_loop:for (int num_itn = 0 ; num_itn < MAX_ITERATION; num_itn++) {
    	bufa_ptr = &bufa[0];
    	bufy_ptr = &bufy[0];

        memcpy(bufa_ptr, a_in_ptr, sizeof(data_t)*LOCAL_DATA_SIZE);
        a_in_ptr+= LOCAL_DATA_SIZE;

        sdx_cppKernel_top_local_data_loop:for (int i = 0; i < LOCAL_DATA_SIZE; i++) {
        #pragma HLS PIPELINE II=1
            simple_kernel (bufa_ptr, bufy_ptr);
            //printf(" __SRAI DBG = bufa = %f :: bufy = %f :: Local_size_i = %d\n", *bufa_ptr, *bufy_ptr, i);
            bufa_ptr++;
            bufy_ptr++;
        }
        bufy_ptr = &bufy[0];


        memcpy(y_out_ptr, bufy_ptr, sizeof(data_t)*LOCAL_DATA_SIZE);
        y_out_ptr += LOCAL_DATA_SIZE;
    }
  return;
}

