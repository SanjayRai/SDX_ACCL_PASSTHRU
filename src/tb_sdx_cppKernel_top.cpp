#include<stdio.h>
#include<math.h>

#include "sdaccel_utils.h" 
#include "sdx_cppKernel_top.h" 




double getCPUTime();

void gen_test_data(data_t *a) {

  static data_t temp = 1.0f;
  for (int i = 0 ; i < GLOBAL_DATA_SIZE; i++) {
      a[i] = temp;
      temp = temp+1.0000;
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
  int i;
  int num_itn;
  int cu;

  int in_args_size[10];               // Array of input Argument sizes
  int out_args_size[10];              // Array of output Argument sizes
  int num_input_args;
  int num_output_args;
  int init_success;
  int run_success;
  int sdaccel_clean_success;
  int in_index;
  int out_index;

  num_input_args = 1;
  num_output_args = 1;
  
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
    //for (num_itn = 0 ; num_itn < MAX_ITERATION; num_itn++) {
        sdx_cppKernel_top(a_in_ptr, a_out_ptr);
        //a_in_ptr+= LOCAL_DATA_SIZE;
        //a_out_ptr+= LOCAL_DATA_SIZE;

    //} // MAX_ITERATION
    endTime = getCPUTime();

     a_in_ptr = a_in_head;
     a_out_ptr = a_out_ptr_head;
#else 
    fpga_hw_accel<data_t,data_t, NUMBER_OF_COMPUTE_UNITS, LOCAL_DATA_SIZE, GLOBAL_DATA_SIZE> Alpha_Data_ku060_card;

    if (argc != 2){
    printf("%s <inputfile>\n", argv[0]);
    return EXIT_FAILURE;
    }
    in_index = 0;
    for (i = 0; i <  num_input_args;i++) { 
        in_args_size[in_index] = sizeof(data_t);
        in_index++;
    }
    out_index = 0;
    for (i = 0; i <  num_output_args;i++) { 
        out_args_size[out_index] = sizeof(data_t);
        out_index++;
    }
    init_success = Alpha_Data_ku060_card.initalize_fpga_hw_accel(argv[1], "sdx_cppKernel_top", in_args_size, out_args_size, num_input_args, num_output_args); 
    if (init_success ) {
        printf ("Launching Algorithm on FPGA Platform\n");
        startTime = getCPUTime();
        


        a_in_ptr = a_in_head;
        a_out_ptr = a_out_ptr_head;
        
        run_success = 0;
        //for (num_itn = 0 ; num_itn < MAX_ITERATION; num_itn++) {
            //printf("DBG__SRAI(top) calling run_fpga_accel - %d\n",num_itn);
            //printf (" ** Starting  Algorithm on FPGA Platform - Iteration : %d\n", num_itn);
            run_success |=  Alpha_Data_ku060_card.run_fpga_accel (a_in_ptr, a_out_ptr);
            //a_in_ptr += (NUMBER_OF_COMPUTE_UNITS*LOCAL_DATA_SIZE);
            //a_out_ptr+= (NUMBER_OF_COMPUTE_UNITS*LOCAL_DATA_SIZE);
            //printf (" ***********************  End of - Iteration : %d\n", num_itn);
        //}

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
    //int NUMBER_OF_COMPUTE_UNITS_to_print = NUMBER_OF_COMPUTE_UNITS;
    //int LOCAL_DATA_SIZE_to_print = LOCAL_DATA_SIZE;
    //int MAX_ITERATION_to_print = MAX_ITERATION;;

    int NUMBER_OF_COMPUTE_UNITS_to_print = 1;
    int LOCAL_DATA_SIZE_to_print = 8;
    int MAX_ITERATION_to_print = 1;

        //a_in_ptr = a_in_head + LOCAL_DATA_SIZE*(MAX_ITERATION - MAX_ITERATION_to_print);
        //a_out_ptr = a_out_ptr_head + LOCAL_DATA_SIZE*(MAX_ITERATION - MAX_ITERATION_to_print);
    a_in_ptr = a_in_head ;
    a_out_ptr = a_out_ptr_head ;

      printf (" ------------   Results  ---------------------------------------------------------------------------\n");
      printf (" Global_data_size: %d(%d MB) :: Local_data_size : %d (%d MB):: Number_of_CU = %d :: Num_iternation = %d\n", GLOBAL_DATA_SIZE, (int)GLOBAL_DATA_SIZE_IN_MB, LOCAL_DATA_SIZE, (int)LOCAL_DATA_SIZE_IN_MB, NUMBER_OF_COMPUTE_UNITS, MAX_ITERATION); 
      printf (" ---------------------------------------------------------------------------------------------------\n");
      printf (" ---------------------------------------------------------------------------------------------------\n");
    ElapsedTime = (endTime - startTime);
    for (num_itn = 0; num_itn <  MAX_ITERATION_to_print;num_itn++) { 
      printf("\n Input Test Data Set : %d:\n", num_itn);
      for (cu =0;cu<NUMBER_OF_COMPUTE_UNITS_to_print;cu++) {
          for (i=0;i<LOCAL_DATA_SIZE_to_print;i++) {
               printf ("\t%d ",(int)*a_in_ptr);
               a_in_ptr++;
           }
           printf ("\n");
      }
    printf("\n**** Result Data Set : ****\n");
      for (cu =0;cu<NUMBER_OF_COMPUTE_UNITS_to_print;cu++) {
        for (i=0;i<LOCAL_DATA_SIZE_to_print;i++) {
            printf ("\t%d ",(int)*a_out_ptr);
            a_out_ptr++;
        }
       printf ("\n");
      }
        printf ("-----------------------------------------------------\n");
    }
    delete [] a_in_head;
    delete [] a_out_ptr_head;
    printf (" Elapsed Time for algorithm = %1f sec\n", ElapsedTime);
    return 0;
}
