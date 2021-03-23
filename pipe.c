#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

int main(int argc, const char * argv[]){
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE] = {0};
	int fd[2] = {0};
	pid_t pid;

	if (pipe(fd) == -1) {
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Failed!\n");
	}
	if (pid > 0) {
		close(fd[READ_END]);
		printf("Process %d write: \t%s\n", getpid(), write_msg);
		write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
	} else {
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("Process %d read: \t%s\n", getpid(), read_msg);
		close(fd[READ_END]);
	}

	return 0;
}
