#ifndef DRAC_ATOMIC_H
#define DRAC_ATOMIC_H

// This library holds all the basic operations to implement a
// hand made multicore algorithm.

#define lock_init()			\
	{				\
		.last_core = (-1),	\
	}

typedef struct {
 int last_core;
} atomic_t;

// This functionreads the content from mhartid csr register to determine the number of hardware thread
static inline int get_core_id(void){
	register int id = 0;

	// subroutine to read the hardware id
	__asm__ __volatile__ (
		"nop\n\t"
		"csrr %[output], mhartid\n\t"
	: [output] "=r" (id)
	: // no input list
	: // no clobber list
	);

	return id;
}

// Reads the content from a global pointer to determine who was the last core to write on this memory address
int read_last_core(atomic_t * lock, int coreID){
	register int local_lock;
	__asm__ __volatile__ (
		"amoswap.w %[output], %[ID], %[lock]\n\t"
		: [output] "=r" (local_lock), [lock] "+A" (lock->last_core)
		: [ID] "r" (coreID)
		: // no clobber list
	);

	return local_lock;

}

// This function it is used as synchronization barrier for 2 cores
// TODO: make this function more generic to be used with n number of cores, and making in a single function to speedup the detection
static inline void sync_system(atomic_t * lock, int coreID){

	register int local_lock;
	// Preload operation
	__asm__ __volatile__ (
		"amoswap.w %[output], %[ID], %[lock]\n\t"
		"fence\n\t"
		: [output] "=r" (local_lock), [lock] "+A" (lock->last_core)
		: [ID] "r" (coreID)
		: // no clobber list
	);
	// sync area
	while(read_last_core(lock, coreID) == coreID);
	__asm__ __volatile__ ("fence");
}

// This function is used for the Core0 to wake up the core1
// TODO: do this function to be independent from the number of cores
static inline void core1_fork(void){
	register int address;
	__asm__ __volatile__ (
		"li %[addr], 4096\n\t"
		"add %[addr], tp, %[addr]\n\t"
		"amoswap.d %[addr], ra, (%[addr])\n\t"
		: [addr] "+r" (address)
		: // no input list
		: // no clobber list
	);
}

// This function wakes up nCores to start execution
static inline void cores_fork(int nCores){
	register int cores; // 4KB
	register int addresses;

	// We calculate and execute for all the cores specified
	for (int i=1; i<nCores; i++){
		__asm__ __volatile__ (
			"li %[addr], 8192\n\t"
			"li %[cor], 4096\n\t"
			"add %[idx], %[idx], -1\n\t"
			"mul %[addr], %[idx], %[addr]\n\t"
			"add %[addr], tp, %[addr]\n\t"
			"add %[addr], %[addr], %[cor]\n\t"
			"amoswap.d %[addr], ra, (%[addr])\n\t"
			: [addr] "+r" (addresses), [cor] "+r" (cores)
			: [idx] "r" (i)
			: // no clobber list
		);
	}
}

// Function to send back to IDLE the core 1
// TODO: re-factor for n number of cores.
void core1_idle(int coreID){
	
	__asm__ __volatile__ (
		"beqz %[ID], aftercore0\n\t"
		"mv sp, s0\n\t"
		"ld ra, 16(sp)\n\t"
		"ret\n\t"
		"aftercore0:\n\t"
		:
		: [ID] "r" (coreID)
		: // no clobber list
	);
}

// It sents the non-core0 active cores to idle state
// TODO: test if this work for n number of cores
void cores_idle(int nCores, int  coreID){

	__asm__ __volatile__ (
		"beqz %[ID], core0passed\n\t"
		"mv sp, s0\n\t"
		"ld ra, 16(sp)\n\t"
		"ret\n\t"
		"core0passed:\n\t"
		:
		: [ID] "r" (coreID)
		: // no clobber list
	);
}

// Barrier function to synchronize all the hardware threads at the same point
void barrier_func(int nCores, int coreID){
static volatile int sense;
  static volatile int count;
  static __thread int threadsense;

  __sync_synchronize();

  threadsense = !threadsense;
  if (__sync_fetch_and_add(&count, 1) == nCores-1)
  {
    count = 0;
    sense = threadsense;
  }
  else while(sense != threadsense)
    ;

__sync_synchronize();

}

// It read the register   which holds the current cycle count of the system
long read_cycle_count(){
	register long cycle_count;
	__asm__ __volatile__ (
		"nop\n\t"
		"rdcycle %[count]\n\t"
		"nop\n\t"
		: [count] "=r" (cycle_count)
		: // no input list
		: // no clobber list
	);

	return cycle_count;
}


#endif
