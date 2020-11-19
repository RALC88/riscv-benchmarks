#include "util.h"
#include "ipc_stats.h"
#include "dataset1.h"

int i;
int histogram[16] = {};

int cores_count;
int core_data_lenght;

int main(int argc, char** argv)
{
    unsigned long cycles1, cycles2, instr2, instr1;

    printf("\n   *** HISTOGRAM BENCHMARK TEST ***\n\n");
    printf("RESULTS OF THE TEST:\n");

    printf("N = %d \n", DATA_SIZE);


// get the number of hardware cores we have available in the system
cores_count = get_core_count();
core_data_lenght = DATA_SIZE/cores_count;

// Printing some info
printf("Executing with : %u", get_core_count());
printf(" Cores\n");

// wakeup all the cores
_cores_fork_(cores_count);

// get the local core hardware id
int coreID = get_core_id();

// Calculate the cores begin and end of the data chunks
int begining = coreID * core_data_lenght;
int ending = (coreID + 1) * core_data_lenght;


  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

	// Execute the program
	for(int x=begining; x<ending; x++){
		histogram[data[x]] += 1;
	}

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();


   printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
   printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
   printf("-CSR   IPC :%d.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

// synchronize the cores execution
barrier(cores_count);

// put the core except the 0 to idle mode
_core_idle_(coreID);

printf("Output : ");

for (int x=0; x<16; x++)
	printf("%u:%u,", x, histogram[x]);

printf("\n");

    return 0;
}
