#include "sdx_cppKernel_top.h"

void simple_kernel (data_t *a_in, data_t *results) {
#pragma HLS PIPELINE
      *results = (*a_in);

  return;

}
