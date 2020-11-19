// See LICENSE for license details.

//**************************************************************************
// Median filter bencmark
//--------------------------------------------------------------------------
//
// This benchmark performs a 1D three element median filter. The
// input data (and reference data) should be generated using the
// median_gendata.pl perl script and dumped to a file named
// dataset1.h You should not change anything except the
// HOST_DEBUG and PREALLOCATE macros for your timing run.

#include "util.h"
#include "median.h"

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
                Median_Per_Second;

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  unsigned long cycles1, cycles2, instr2, instr1;
  int Run_Index;
  int Number_Of_Runs = NUMBER_OF_RUNS;

  int results_data[DATA_SIZE];

  // Output the input array
  printArray( "input",  DATA_SIZE, input_data  );
  printArray( "verify", DATA_SIZE, verify_data );

#if PREALLOCATE
  // If needed we preallocate everything in the caches
  median( DATA_SIZE, input_data, results_data );
#endif

  // Do the filter
  //setStats(1);
  //Start_Timer();
  instr1 = get_inst_count();
  cycles1 = get_cycles_count();
  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index){
     median( DATA_SIZE, input_data, results_data );
  }
  instr2 = get_inst_count();
  cycles2 = get_cycles_count();
  //Stop_Timer();
  //setStats(0);

  printf("-CSR   NUMBER OF EXEC CYCLES :%lu\n", cycles2 - cycles1);
  printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%lu\n", instr2 - instr1);
  printf("-CSR   IPC :%lu.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

  // Print simulation time
  //User_Time = End_Time - Begin_Time;
  //  Microseconds = ((User_Time / Number_Of_Runs) * Mic_secs_Per_Second) / HZ;
  //Microseconds = (User_Time / Number_Of_Runs) * (Mic_secs_Per_Second / HZ); /* TODO: pevious was generating an exception on lagarto */
  //Median_Per_Second = (HZ * Number_Of_Runs) / User_Time;

  //printf("Microseconds for one run through Median: %ld\n", Microseconds);
  //printf("Medians per Second:                      %ld\n", Median_Per_Second);
  // Print out the results
  printArray( "results", DATA_SIZE, results_data );

  // Check the results
  return verify( DATA_SIZE, results_data, verify_data );
}
