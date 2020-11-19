#=======================================================================
# UCB CS250 Makefile fragment for benchmarks
#-----------------------------------------------------------------------
#
# Each benchmark directory should have its own fragment which
# essentially lists what the source files are and how to link them
# into an riscv and/or host executable. All variables should include
# the benchmark name as a prefix so that they are unique.
#

matrix_mult_c_src = \
	matrix_mult.c \
	syscalls.c \

matrix_mult_riscv_src = \
	crt.S \

matrix_mult_c_objs     = $(patsubst %.c, %.o, $(matrix_mult_c_src))
matrix_mult_riscv_objs = $(patsubst %.S, %.o, $(matrix_mult_riscv_src))

matrix_mult_host_bin = matrix_mult.host
$(matrix_mult_host_bin) : $(matrix_mult_c_src)
	$(HOST_COMP) $^ -o $(matrix_mult_host_bin)

matrix_mult_riscv_bin = matrix_mult.riscv
$(matrix_mult_riscv_bin) : $(matrix_mult_c_objs) $(matrix_mult_riscv_objs)
	$(RISCV_LINK) $(matrix_mult_c_objs) $(matrix_mult_riscv_objs) -o $(matrix_mult_riscv_bin) $(RISCV_LINK_OPTS)

junk += $(matrix_mult_c_objs) $(matrix_mult_riscv_objs) \
        $(matrix_mult_host_bin) $(matrix_mult_riscv_bin)
