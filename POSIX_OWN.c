#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>

#define PERM S_IRUSR | S_IWUSR
#define MAX_SEQUENCE 10

typedef struct Shared_data{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
}shared_data;   //定义类型为share_data的类型

int main(int argc, char *argv[])
{
    key_t shmid;
    shared_data *p_address, *c_address;  //定义父进程和子进程访问共同内存的指针
    pid_t pid;

    if (argc != 2) { //检查输入合法性
        fprintf(stderr, "Usage:./a.out fib_size\n");
        exit(1);
    }
    if (atof(argv[1]) > 10 || atof(argv[1]) < 1) {
        fprintf(stderr, "error:please input correctly\n");
        exit(1);
    }

    shared_data store;      //暂时存储share_data
    store.sequence_size = atof(argv[1]);


	if ((shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1) {  //分配内存共享区
        fprintf(stderr, "Create share momery error:%s\n", strerror(errno));
        exit(1);
    }
    p_address = shmat(shmid, 0, 0);
    memset(p_address, '\0', sizeof(shared_data));  //清零

    for (int i = 0; i < MAX_SEQUENCE; i++) {
        p_address->fib_sequence[i] = store.fib_sequence[i];
    
	}
    p_address->sequence_size = store.sequence_size;

    pid = fork();       //创建子进程
    if (pid > 0) {      //父进程
        wait(NULL);
        for (int i = 0; i < p_address->sequence_size; i++) {
            printf("%ld ", (p_address->fib_sequence[i]));
		}

        exit(0);
    } else if (pid == 0) {      //子进程
        c_address = shmat(shmid, 0, 0);
        c_address->fib_sequence[0] = 1;
        c_address->fib_sequence[1] = 1;
        for (int i = 2; i < c_address->sequence_size; i++) {
            c_address->fib_sequence[i] = c_address->fib_sequence[i - 1] + c_address->fib_sequence[i - 2];
        }

        exit(0);
    

	}
    return 0;
}
