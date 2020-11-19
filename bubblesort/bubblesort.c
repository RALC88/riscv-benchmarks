#include <sys/types.h>
#include <sys/times.h>

#include "util.h"
#include "ipc_stats.h"
#include "dataset1.h"

int main (void) 
{
unsigned long cycles1, cycles2, instr2, instr1;
int i,j,k; 

unsigned long aux;

   printf("\n   *** BUBBLE SORT BENCHMARK TEST ***\n\n");
   printf("Size of the vector:%d\n",DATA_SIZE);

  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

	for (i=0;i<DATA_SIZE;i++) 
	{ 
		for (j=0;j<DATA_SIZE-1;j++) 
		{ 
		if (input_data[j]>=input_data[j+1]) 
			{ 
			aux=input_data[j]; 
			input_data[j]=input_data[j+1]; 
			input_data[j+1]=aux; 
			} 
		} 
	} 

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

	int error=0;

	for (i=0;i<DATA_SIZE;i++) 
	{ 
		if (input_data[i] != verify_data[i])
		{
            error = 1;
            printf("The program failed in element %d ,input_data %d , verify_data %d \n", i,input_data[i] ,verify_data[i]);
			return error;
		}
	}

   printf("Successful\n\n");
   printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
   printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
   printf("-CSR   IPC :%d.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

return error; 
} 
