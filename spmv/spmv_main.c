// See LICENSE for license details.

//**************************************************************************
// Double-precision general matrix multiplication benchmark
//--------------------------------------------------------------------------

#include "util.h"

//--------------------------------------------------------------------------
// Input/Reference Data
#include "ipc_stats.h"
#include "dataset1.h"

void spmv(int r, const long* val, const int* idx, const long* x,
          const int* ptr, long* y)
{
  for (int i = 0; i < r; i++)
  {
    int k;
    long yi0 = 0, yi1 = 0, yi2 = 0, yi3 = 0;
    for (k = ptr[i]; k < ptr[i+1]-3; k+=4)
    {
      yi0 += val[k+0]*x[idx[k+0]];
      yi1 += val[k+1]*x[idx[k+1]];
      yi2 += val[k+2]*x[idx[k+2]];
      yi3 += val[k+3]*x[idx[k+3]];
    }
    for ( ; k < ptr[i+1]; k++)
    {
      yi0 += val[k]*x[idx[k]];
    }
    y[i] = (yi0+yi1)+(yi2+yi3);
  }
}

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{

unsigned long cycles1, cycles2, instr2, instr1;
int Run_Index;
int Number_Of_Runs = NUMBER_OF_RUNS;

long y[R];

#if PREALLOCATE
  spmv(R, val, idx, x, ptr, y);
#endif

  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

  //for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index){
     spmv(R, val, idx, x, ptr, y);
  //}

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

  printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
  printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
  printf("-CSR   IPC :%d.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

  // Check the results
  return verify(R, y, verify_data);
}
