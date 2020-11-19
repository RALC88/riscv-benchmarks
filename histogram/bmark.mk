#=======================================================================
# UCB CS250 Makefile fragment for benchmarks
#-----------------------------------------------------------------------
#
# Each benchmark directory should have its own fragment which
# essentially lists what the source files are and how to link them
# into an riscv and/or host executable. All variables should include
# the benchmark name as a prefix so that they are unique.
#

histogram_c_src = \
	histogram.c \
	syscalls.c \

histogram_riscv_src = \
	crt.S \

histogram_c_objs     = $(patsubst %.c, %.o, $(histogram_c_src))
histogram_riscv_objs = $(patsubst %.S, %.o, $(histogram_riscv_src))

histogram_host_bin = histogram.host
$(histogram_host_bin) : $(histogram_c_src)
	$(HOST_COMP) $^ -o $(histogram_host_bin)

histogram_riscv_bin = histogram.riscv
$(histogram_riscv_bin) : $(histogram_c_objs) $(histogram_riscv_objs)
	$(RISCV_LINK) $(histogram_c_objs) $(histogram_riscv_objs) -o $(histogram_riscv_bin) $(RISCV_LINK_OPTS)

junk += $(histogram_c_objs) $(histogram_riscv_objs) \
        $(histogram_host_bin) $(histogram_riscv_bin)
