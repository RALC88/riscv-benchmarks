This foldar contains a benchmark base implementation to test c codes 
over the lagarto architecture in the DRAC project. This implementation 
include all the utilities to run single and multi-core systems. The 
basic implementation is based on the last rocket-chip release, with 
some minor changes.
The most important change applied is on the common/crt.S file to make
it match with our current configuration, also this file follows the 
structure from the file that can he found at the path folder:
$TOP/fpga/board/kc705/driver which is used for the FPGA implementation
that can boot linux.
The second change is the usage of 2 different last jump address to point
a different implementation. This it mean that depending on a simple 
"#define " flag we can select the default rocket chip implementation which
overwrites the main and thread_entry implementations, and a second custom
implementation that uses a simple "int main(void)" constructor to
facilitate the integration of simple c programs, all this to enable
the users to code without thinking on the tool format and include simple
c programs to test hardware.
