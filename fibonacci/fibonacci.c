/*
 * Copyright (c) 2005-2018 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//#include <stdio.h>
//#include <stdlib.h>

#include <sys/types.h>
#include <sys/times.h>
#include "util.h"

#include "ipc_stats.h"

static int fib(int i) {return (i>1) ? fib(i-1) + fib(i-2) : i;}

int main(void) {
    unsigned long cycles1, cycles2, instr2, instr1;
    int aux,i,j;
    int result; 
    int n=20;
    int A[20];
    int error=0;

    printf("\n   *** FIBONACCI BENCHMARK TEST ***\n\n");
    printf("RESULTS OF THE TEST:\n");

    printf("N = %d \n", n);

  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

    for(j=0; j<n; j++) {
        result=fib(j);
        A[j] = result;
    }

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

   printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
   printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
   printf("-CSR   IPC :%d.%d\n\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

	printf("Vector Ordenado:\n");
	for (i=0;i<n;i++) { 
	    printf("%d ",A[i]);
	}
	printf("\n");

return error; 
} 
