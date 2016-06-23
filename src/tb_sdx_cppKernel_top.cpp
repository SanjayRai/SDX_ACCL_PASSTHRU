#include<stdio.h>
#include<math.h>

#include "sdaccel_utils.h" 
#include "sdx_cppKernel_top.h" 




double getCPUTime();

void gen_test_data(data_t *a) {

  //static data_t temp = 1.0f;
  static data_t temp = 1;
  for (int i = 0 ; i < GLOBAL_DATA_SIZE; i++) {
      a[i] = temp;
      temp = temp+1;
  }
}


int main(int argc, char** argv)
{

  double startTime;
  double endTime;
  double ElapsedTime;

  data_t *a_in_ptr;
  data_t *a_in_head;
  data_t *a_out_ptr;
  data_t *a_out_ptr_head;
  int num_itn;

  int in_args_size[10];               // Array of input Argument sizes
  int out_args_size[10];              // Array of output Argument sizes
  int init_success;
  int run_success;
  int sdaccel_clean_success;

  bool RESULT_UNSUCESSFULL;

  a_in_ptr = new data_t[GLOBAL_DATA_SIZE];
  a_in_head = a_in_ptr;
  a_out_ptr = new data_t[GLOBAL_DATA_SIZE];
  a_out_ptr_head = a_out_ptr;


  printf("-------------------------------------------------------------\n");
  printf("--------Initialize the Test vector space---------------------\n");
  printf("-------------------------------------------------------------\n");
    for (num_itn = 0 ; num_itn < GLOBAL_DATA_SIZE; num_itn++) {
        *a_in_ptr = 0.0f; 
        *a_out_ptr = 1.111111f; 
        a_in_ptr++;
        a_out_ptr++;
    }

     a_in_ptr = a_in_head;
     a_out_ptr = a_out_ptr_head;

  printf("-------------------------------------------------------------\n");
  printf("Create Test Data Set\n");
  printf("Note GLOBAL_DATA_SIZE (Number of test loops run) = %d\n", GLOBAL_DATA_SIZE);
  printf("-------------------------------------------------------------\n\n\n");

 gen_test_data(a_in_ptr);
  
 a_in_ptr = a_in_head;

    //-----------------------------------------------
#ifdef GPP_ONLY_FLOW  
    startTime = getCPUTime();
    sdx_cppKernel_top(a_in_ptr, a_out_ptr);
    endTime = getCPUTime();

     a_in_ptr = a_in_head;
     a_out_ptr = a_out_ptr_head;
#else 
    fpga_hw_accel<data_t,data_t, 1, LOCAL_DATA_SIZE> Alpha_Data_ku060_card;

    if (argc != 2){
    printf("%s <inputfile>\n", argv[0]);
    return EXIT_FAILURE;
    }

    in_args_size[0] = sizeof(data_t)*LOCAL_DATA_SIZE;
    out_args_size[0] = sizeof(data_t)*LOCAL_DATA_SIZE;

    init_success = Alpha_Data_ku060_card.initalize_fpga_hw_accel(argv[1], "sdx_cppKernel_top", in_args_size, out_args_size, 1, 1); 
    if (init_success ) {
        printf ("Launching Algorithm on FPGA Platform\n");
        startTime = getCPUTime();
        


        a_in_ptr = a_in_head;
        a_out_ptr = a_out_ptr_head;
        
        run_success = 0;
        for (int itn = 0; itn < MAX_ITERATION; itn++) {
            run_success |=  Alpha_Data_ku060_card.run_fpga_accel (a_in_ptr, a_out_ptr);
            a_in_ptr += LOCAL_DATA_SIZE;
            a_out_ptr += LOCAL_DATA_SIZE;
        }

        a_in_ptr = a_in_head;
        a_out_ptr = a_out_ptr_head;


        endTime = getCPUTime();
        if (!run_success ) {
            printf("Error: SdAccel CPP Kernel execution Failed !!\n");
        }
    } else {
        printf("Error: SdAccel provisioning Failed !!\n");
    }
    sdaccel_clean_success = Alpha_Data_ku060_card.clean_fpga_hw_accel();
    if (!sdaccel_clean_success) {
        printf("Error: SdAccel resource cleanip Failed !!\n");
    }

#endif

    int LOCAL_DATA_SIZE_to_print = 20;
    int MAX_ITERATION_to_print = 1;


    RESULT_UNSUCESSFULL = 0;
    for (num_itn = 0 ; num_itn < GLOBAL_DATA_SIZE; num_itn++) {
        if (*a_in_ptr == *a_out_ptr){
            RESULT_UNSUCESSFULL |= 0;
        } else {
            RESULT_UNSUCESSFULL |= 1;
        }
        if (RESULT_UNSUCESSFULL) {
            //printf("Result verification faild expected %f got %f at %d\n", (double)*a_in_ptr, (double)*a_out_ptr, num_itn);
        }
        a_in_ptr++;
        a_out_ptr++;
    }

   a_in_ptr = a_in_head;
   a_out_ptr = a_out_ptr_head;

    a_in_ptr = a_in_head;
    a_out_ptr = a_out_ptr_head;
    if (RESULT_UNSUCESSFULL) {
        printf (" ------------   Results did not Verify - Test Failed !!!!!! -------------------------------------------------\n");
    } else {
        printf (" ------------   Results Verified  ---------------------------------------------------------------------------\n");
        printf (" ------------   Results  ---------------------------------------------------------------------------\n");
        printf (" Global_data_size: %d(%d MB) :: Local_data_size : %d (%d MB)\n", GLOBAL_DATA_SIZE, (int)GLOBAL_DATA_SIZE_IN_MB, LOCAL_DATA_SIZE, (int)LOCAL_DATA_SIZE_IN_MB); 
        printf (" ---------------------------------------------------------------------------------------------------\n");
        printf (" ---------------------------------------------------------------------------------------------------\n");
        ElapsedTime = (endTime - startTime);
        for (num_itn = 0; num_itn <  MAX_ITERATION_to_print;num_itn++) { 
            printf("\n Input Test Data Set : %d:\n", num_itn);
                for (int i=0;i<LOCAL_DATA_SIZE_to_print;i++) {
                    printf ("\t%d ",(int)*a_in_ptr);
                    a_in_ptr++;
                }
                printf ("\n");
            printf("\n**** Result Data Set : ****\n");
                for (int i=0;i<LOCAL_DATA_SIZE_to_print;i++) {
                    printf ("\t%d ",(int)*a_out_ptr);
                    a_out_ptr++;
                }
                printf ("\n");
            printf ("-----------------------------------------------------\n");
        }
    }
    delete [] a_in_head;
    delete [] a_out_ptr_head;
    printf (" Elapsed Time for algorithm = %1f sec\n", ElapsedTime);
    return 0;
}
