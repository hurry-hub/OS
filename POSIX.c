#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(){
	int shm_id;	
	char * shared_memory;
	const int size = 4096;
	pid_t pid;
	printf("Hello World!\n");
	shm_id = shmget(IPC_PRIVATE, size, IPC_CREAY);
	if (shm_id < 0) {
		perror("get shm ipc_id error");
		return-1;
	}
	pid = fork();
	if (pid == 0) {
		printf("Child Process: %d\n", getpid());
		shared_memory = (char *) shmat(shm_id, NULL, 0);
		sprintf(shared_memory, "Hi I'm Child Process!\n");
		shmdt(shared_memory);
		return 0;
	} else if (pid > 0) {
		sleep(10);
		printf("Parent Process: %d\n", getpid());
		shared_memory = (char *) shmat(shm_id, NULL, 0);
		printf("Parent Process: %s", shared_memory);
		shmdt(shared_memory);
		shmctl(shm_id, IPC_RMID, NULL);
		wait(NULL);
		printf("Parent Process Exits: %d\n", getpid());
		return 0;
	} else {
		perror("fork error");
		shmctl(shm_id, IPC_RMID, NULL);
		return -1;
	}
		
	return 0;
}
