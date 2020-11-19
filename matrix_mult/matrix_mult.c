/* $Id: matmult.c,v 1.2 2005/04/04 11:34:58 csg Exp $ */

/* matmult.c */
/* was mm.c! */


/*----------------------------------------------------------------------*
 * To make this program compile under our assumed embedded environment,
 * we had to make several changes:
 * - Declare all functions in ANSI style, not K&R.
 *   this includes adding return types in all cases!
 * - Declare function prototypes
 * - Disable all output
 * - Disable all UNIX-style includes
 *
 * This is a program that was developed from mm.c to matmult.c by
 * Thomas Lundqvist at Chalmers.
 *----------------------------------------------------------------------*/
//#define UPPSALAWCET 1


/* ***UPPSALA WCET***:
   disable stupid UNIX includes */
#ifndef UPPSALAWCET
#include <sys/types.h>
#include <sys/times.h>
#endif

#include "ipc_stats.h"

#include "util.h"

/*
 * MATRIX MULTIPLICATION BENCHMARK PROGRAM:
 * This program multiplies 2 square matrices resulting in a 3rd
 * matrix. It tests a compiler's speed in handling multidimensional
 * arrays and simple arithmetic.
 */

#define UPPERLIMIT 12

typedef int matrix [UPPERLIMIT][UPPERLIMIT];

int Seed;
matrix ArrayA, ArrayB, ResultArray;


#ifdef UPPSALAWCET
/* Our picky compiler wants prototypes! */
void Multiply(matrix A, matrix B, matrix Res);
void InitSeed(void);
void Test(matrix A, matrix B, matrix Res);
void Initialize(matrix Array);
int RandomInteger(void);
void PrintMatrix(A);
#endif

int main()
{
   unsigned long cycles1, cycles2, instr2, instr1;
   InitSeed();

   printf("\n   *** MATRIX MULTIPLICATION BENCHMARK TEST ***\n\n");
   printf("RESULTS OF THE TEST:\n");

  instr1 = get_inst_count();
  cycles1 = get_cycles_count();

  Test(ArrayA, ArrayB, ResultArray);

  instr2 = get_inst_count();
  cycles2 = get_cycles_count();

   printf("-	SIZE OF ARRAY IS : %d\n", UPPERLIMIT);
   printf("-CSR   NUMBER OF EXEC CYCLES :%d\n", cycles2 - cycles1);
   printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%d\n", instr2 - instr1);
   printf("-CSR   IPC :%d.%d\n", (instr2 - instr1)/(cycles2 - cycles1),100*(instr2 - instr1)/(cycles2 - cycles1)%100);

   printf("\nArrayA:\n");
   PrintMatrix(ArrayA);
   printf("ArrayB:\n");
   PrintMatrix(ArrayB);
   printf("ResultArray:\n");
   PrintMatrix(ResultArray);
}


void PrintMatrix(matrix X)
{  
   for (int i = 0; i < UPPERLIMIT; ++i)
   {
    printf("|");
     for (int j = 0; j < UPPERLIMIT; ++j)
     {
       printf("%d   ",X[i][j] );
     }
     printf("|\n");
   }
   printf("\n");
}

void InitSeed(void)
/*
 * Initializes the seed used in the random number generator.
 */
{
  /* ***UPPSALA WCET***:
     changed Thomas Ls code to something simpler.
   Seed = KNOWN_VALUE - 1; */
  Seed = 0;
}


void Test(matrix A, matrix B, matrix Res)
/*
 * Runs a multiplication test on an array.  Calculates and prints the
 * time it takes to multiply the matrices.
 */
{
#ifndef UPPSALAWCET
   long StartTime, StopTime;
//   float TotalTime;
   long TotalTime;
#endif

   Initialize(A);
   Initialize(B);

   /* ***UPPSALA WCET***: don't print or time */
#ifndef UPPSALAWCET
//   StartTime = ttime ();
#endif

   Multiply(A, B, Res);
   
/* ***UPPSALA WCET***: don't print or time */
#ifndef UPPSALAWCET

//   StopTime = ttime();
//   TotalTime = (StopTime - StartTime) / 1000.0;

//   printf("    - Size of array is %d\n", UPPERLIMIT);
//   printf("    - Total multiplication time is %3.3ld seconds\n\n", TotalTime);
#endif

}


void Initialize(matrix Array)
/*
 * Intializes the given array with random integers.
 */
{
   int OuterIndex, InnerIndex;

   for (OuterIndex = 0; OuterIndex < UPPERLIMIT; OuterIndex++)
      for (InnerIndex = 0; InnerIndex < UPPERLIMIT; InnerIndex++)
         //Array[OuterIndex][InnerIndex] = RandomInteger();
         Array[OuterIndex][InnerIndex] = 10;
         //Array[OuterIndex][InnerIndex] = (OuterIndex*UPPERLIMIT)+1+InnerIndex;//(OuterIndex*(InnerIndex+1))+InnerIndex+1;
}


int RandomInteger(void)
/*
 * Generates random integers between 0 and 8095
 */
{
   Seed = ((Seed * 133) + 81) % 8095;
   return (Seed);
}


#ifndef UPPSALAWCET
int ttime()
/*
 * This function returns in milliseconds the amount of compiler time
 * used prior to it being called.
 */
{
   struct tms buffer;
   int utime;

   /*   times(&buffer);   times not implemented */
   utime = (buffer.tms_utime / 60.0) * 1000.0;
   return (utime);
}
#endif

void Multiply(matrix A, matrix B, matrix Res)
/*
 * Multiplies arrays A and B and stores the result in ResultArray.
 */
{
   register int Outer, Inner, Index;

   for (Outer = 0; Outer < UPPERLIMIT; Outer++)
      for (Inner = 0; Inner < UPPERLIMIT; Inner++)
      {
         Res [Outer][Inner] = 0;
         for (Index = 0; Index < UPPERLIMIT; Index++)
            Res[Outer][Inner]  +=
               A[Outer][Index] * B[Index][Inner];
       }
//printf("%d\n", Res[10][10]);
}
