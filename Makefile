#=======================================================================
# Makefile for drac tests
#-----------------------------------------------------------------------
orig_dir := .

include $(orig_dir)/Makefrag

default: all

CC=riscv64-unknown-elf-gcc
DUMP=riscv64-unknown-elf-objdump
CFLAGS=-DENTROPY=11275 -static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles
EXTRAFLAGS=-static -Wa,-march=RVIMAFD -std=gnu99 -g -ffast-math -fno-common -fno-builtin-printf
DUMPFLAGS= --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.text.init --section=.data
INCLU=-I./macros/scalar -I./common
LINKER=-T./common/linker.ld
LINKERFLAGS=-ffast-math -lc -lgcc
# Variable to define the kind of implementation
BENCHMARK ?= 0

%.hex: %
	(elf2hex 16 8192 $< 2> /dev/null || elf2hex 16 16384 $<) > $@

%.dump: %
	 $(DUMP) $(DUMPFLAGS) $< > $@

define compile_template

# Build object
$$($(1)_smp_tests): $(1)-smp-%: %.c common/crt.o common/syscalls.c common/smp.h
	$(CC) $(CFLAGS) $(EXTRAFLAGS) $(INCLU) $(LINKER) $$< common/syscalls.c -o $$@ $(LINKERFLAGS) -D__BENCH=$(BENCHMARK)
$(1)_tests += $$($(1)_smp_tests)

.PHONY: $(1)

tests += $$($(1)_tests)

endef

$(eval $(call compile_template,drac))

tests_dump = $(addsuffix .dump, $(tests))
tests_hex = $(addsuffix .hex, $(tests))

# Dependenciess
common/crt.o: common/crt.S
	$(CC) $(EXTRAFLAGS) -c $< -o $@ -D__BENCH=$(BENCHMARK)

junk += $(tests) $(tests_dump) $(tests_hex) common/crt.o

#-------------------------------------------------------------
# default
all: $(tests_dump) $(tests_hex)

clean:
	rm -rf $(junk)
