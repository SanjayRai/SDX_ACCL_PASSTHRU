#include <string.h>

#include "sdx_cppKernel_top.h"
#include <stdio.h>

#include "hls_stream.h"

void sdx_cppKernel_top(data_t a_in[LOCAL_DATA_SIZE], data_t y_out[LOCAL_DATA_SIZE])
{
#pragma HLS INTERFACE m_axi port=a_in offset=slave bundle=gmem depth=1024*64*4
#pragma HLS INTERFACE m_axi port=y_out offset=slave bundle=gmem depth=1024*64*4
#pragma HLS INTERFACE s_axilite port=a_in bundle=control
#pragma HLS INTERFACE s_axilite port=y_out bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control


    data_t bufa_val;
    data_t bufy_val;

    hls::stream<data_t> bufa_strm;
    hls::stream<data_t> bufy_strm;

    sdx_cppKernel_top_local_data_loop:for (int i = 0; i < LOCAL_DATA_SIZE; i++) {
    #pragma HLS PIPELINE II=1
        bufa_strm.write(a_in[i]);
        bufa_val = bufa_strm.read();

        simple_kernel (&bufa_val, &bufy_val);

        bufy_strm.write(bufy_val);
        y_out[i] = bufy_strm.read();
    }
    return;
}

