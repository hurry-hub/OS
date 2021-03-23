#include <stdio.h>
#include <stdlib.h>
int main(){
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		printf("error!\n");
	} else if (pid == 0) {
		execlp("/bin/ls", "ls", NULL);
	} else {
		wait(NULL);
		printf("Child Complete");
		exit(0);
	}
}
