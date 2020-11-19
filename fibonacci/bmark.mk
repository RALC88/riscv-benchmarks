#=======================================================================
# UCB CS250 Makefile fragment for benchmarks
#-----------------------------------------------------------------------
#
# Each benchmark directory should have its own fragment which
# essentially lists what the source files are and how to link them
# into an riscv and/or host executable. All variables should include
# the benchmark name as a prefix so that they are unique.
#

fibonacci_c_src = \
	fibonacci.c \
	syscalls.c \

fibonacci_riscv_src = \
	crt.S \

fibonacci_c_objs     = $(patsubst %.c, %.o, $(fibonacci_c_src))
fibonacci_riscv_objs = $(patsubst %.S, %.o, $(fibonacci_riscv_src))

fibonacci_host_bin = fibonacci.host
$(fibonacci_host_bin) : $(fibonacci_c_src)
	$(HOST_COMP) $^ -o $(fibonacci_host_bin)

fibonacci_riscv_bin = fibonacci.riscv
$(fibonacci_riscv_bin) : $(fibonacci_c_objs) $(fibonacci_riscv_objs)
	$(RISCV_LINK) $(fibonacci_c_objs) $(fibonacci_riscv_objs) -o $(fibonacci_riscv_bin) $(RISCV_LINK_OPTS)

junk += $(fibonacci_c_objs) $(fibonacci_riscv_objs) \
        $(fibonacci_host_bin) $(fibonacci_riscv_bin)
