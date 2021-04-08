#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	#pragma omp parallel
	{
		printf("I am a parallel region.");
	}

	return 0;
}
