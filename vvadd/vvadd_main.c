// See LICENSE for license details.

//**************************************************************************
// Vector-vector add benchmark
//--------------------------------------------------------------------------
//
// This benchmark uses adds to vectors and writes the results to a
// third vector. The input data (and reference data) should be
// generated using the vvadd_gendata.pl perl script and dumped
// to a file named dataset1.h The smips-gcc toolchain does not
// support system calls so printf's can only be used on a host system,
// not on the smips processor simulator itself. You should not change
// anything except the HOST_DEBUG and PREALLOCATE macros for your timing
// runs.
 
#include "util.h"
#include "ipc_stats.h"
//--------------------------------------------------------------------------
// Input/Reference Data

#include "dataset1.h"

//--------------------------------------------------------------------------
// vvadd function

void vvadd( int n, int a[], int b[], int c[] )
{
  int i;
  for ( i = 0; i < n; i++ )
    c[i] = a[i] + b[i];
}

//--------------------------------------------------------------------------
// variables to messure the simulation time
long            Begin_Time,
                End_Time,
                User_Time;
long            Microseconds,
                Vvadd_Per_Second;

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  unsigned long cycles1, cycles2, instr2, instr1;
  int Run_Index;
  int Number_Of_Runs = NUMBER_OF_RUNS;

  int results_data[DATA_SIZE];

  // Output the input array
  printArray( "input1", DATA_SIZE, input1_data );
  printArray( "input2", DATA_SIZE, input2_data );
  printArray( "verify", DATA_SIZE, verify_data );

#if PREALLOCATE
  // If needed we preallocate everything in the caches
  vvadd( DATA_SIZE, input1_data, input2_data, results_data );
#endif

  // Do the vvadd
  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

  //setStats(1);
  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index){
     vvadd( DATA_SIZE, input1_data, input2_data, results_data );
  }

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

  printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
  printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
  printf("-CSR   IPC :%d.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

  // Print out the results
  printArray( "results", DATA_SIZE, results_data );

  // Check the results
  return verify( DATA_SIZE, results_data, verify_data );
}
