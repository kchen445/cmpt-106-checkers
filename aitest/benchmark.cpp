#include "benchmark.hpp"

namespace benchmark {
	long long int aicreate = 0;
	long long int aisolve = 0;
	long long int aimove = 0;
	long long int movecalc = 0;

	void start(int *temp) {
		asm volatile (
	    "cpuid\n\t"
	    "rdtscp\n\t"
	    "movl %%eax, %0\n\t"
	    : "=r" (*temp)
	    : 
	    : "rax", "rbx", "rcx", "rdx"
		);
	}

	int end(int *temp) {
		int end;
	    asm volatile (
	    "cpuid\n\t"
	    "rdtscp\n\t"
	    "movl %%eax, %0\n\t"
	    : "=r" (end)
	    : 
	    : "rax", "rbx", "rcx", "rdx"
		);

		return end-*temp;
	}
}