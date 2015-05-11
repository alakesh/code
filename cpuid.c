/* Program to quickly run cpuid instruction on current processor core
 * By <Alakesh.Haloi@gmail.com>
 **/

#include <stdio.h>
#include <inttypes.h>

int main(int argc, char *argv[])
{
	uint32_t num = 1;
	uint32_t version;
	uint32_t features_1;
	uint32_t features_2;

	__asm__ (	"mov %%eax, %3\n\t"
			"cpuid\n\t"
			"mov %0, %%eax\n\t"
			"mov %1, %%eax\n\t"
			"mov %2, %%ecx"
			: "=o"(version),"=o"(features_1), "=o"(features_2)
			: "o"(num)
			: 
	    );	
	printf("Processor version: 0x%x features_1: 0x%08x features_2: "
		"0x%08x \n", version, features_1, features_2);
	return 0;
}
