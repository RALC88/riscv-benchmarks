#=======================================================================
# UCB CS250 Makefile fragment for benchmarks
#-----------------------------------------------------------------------
#
# Each benchmark directory should have its own fragment which
# essentially lists what the source files are and how to link them
# into an riscv and/or host executable. All variables should include
# the benchmark name as a prefix so that they are unique.
#

bubblesort_c_src = \
	bubblesort.c \
	syscalls.c \

bubblesort_riscv_src = \
	crt.S \

bubblesort_c_objs     = $(patsubst %.c, %.o, $(bubblesort_c_src))
bubblesort_riscv_objs = $(patsubst %.S, %.o, $(bubblesort_riscv_src))

bubblesort_host_bin = bubblesort.host
$(bubblesort_host_bin) : $(bubblesort_c_src)
	$(HOST_COMP) $^ -o $(bubblesort_host_bin)

bubblesort_riscv_bin = bubblesort.riscv
$(bubblesort_riscv_bin) : $(bubblesort_c_objs) $(bubblesort_riscv_objs)
	$(RISCV_LINK) $(bubblesort_c_objs) $(bubblesort_riscv_objs) -o $(bubblesort_riscv_bin) $(RISCV_LINK_OPTS)

junk += $(bubblesort_c_objs) $(bubblesort_riscv_objs) \
        $(bubblesort_host_bin) $(bubblesort_riscv_bin)
