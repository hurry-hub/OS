#include <stdio.h>
#include <stdlib.h>
int main(){
	int i, p_id;
	if ((p_id = fork()) == 0) {
		for (i = 1; i < 3; i++) {
			printf("This is child process\n");
		}
	} else if (p_id < 0) {
		printf("fork new process error!\n");
	   exit(-1);	
	} else {
		for (i = 1; i < 3; i++) {
			printf("This is parent process\n");
		}
	}
}
