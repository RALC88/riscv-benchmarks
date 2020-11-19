// See LICENSE for license details.

// *************************************************************************
// multiply filter bencmark
// -------------------------------------------------------------------------
//
// This benchmark tests the software multiply implemenation. The
// input data (and reference data) should be generated using the
// multiply_gendata.pl perl script and dumped to a file named
// dataset1.h You should not change anything except the
// HOST_DEBUG and VERIFY macros for your timing run.

#include "util.h"

#include "multiply.h"

#include "ipc_stats.h"
//--------------------------------------------------------------------------
// Input/Reference Data

#include "dataset1.h"

//--------------------------------------------------------------------------
// variables to messure the simulation time
long            Begin_Time,
                End_Time,
                User_Time;
long            Microseconds,
                Multiply_Per_Second;

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  unsigned long cycles1, cycles2, instr2, instr1;
  int Run_Index;
  int Number_Of_Runs = NUMBER_OF_RUNS;

  int i;
  int results_data[DATA_SIZE];

  // Output the input arrays
  printArray( "input1",  DATA_SIZE, input_data1  );
  printArray( "input2",  DATA_SIZE, input_data2  );
  printArray( "verify", DATA_SIZE, verify_data );

#if PREALLOCATE
  for (i = 0; i < DATA_SIZE; i++)
  {
    results_data[i] = multiply( input_data1[i], input_data2[i] );
  }
#endif

  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index){
     for (i = 0; i < DATA_SIZE; i++)
     {
        results_data[i] = multiply( input_data1[i], input_data2[i] );
     }
  }
  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

  printf("-CSR   NUMBER OF EXEC CYCLES :%lu\n", cycles2 - cycles1);
  printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%lu\n", instr2 - instr1);
  printf("-CSR   IPC :%lu.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

  // Print out the results
  printArray( "results", DATA_SIZE, results_data );

  // Check the results
  return verify( DATA_SIZE, results_data, verify_data );
}
