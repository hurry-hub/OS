#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#define PERM S_IRUSR|S_IWUSR    //IPC_PRIVATE            保证使用唯一ID(或key)
                                //S_IRUSR | S_IWUSR      使当前用户可以读写这个区域
 
#define MAX_SEQUENCE 10
 
typedef struct Shared_data   //定义结构体  而且保证不用写struct
{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
}shared_data;  //创建一个别名 为shared_data的类型
 
int main(int argc, char **argv)
{
    //step1------------------------------------------
    key_t shmid;   //定义shmid
    shared_data *p_addr, *c_addr;  //定义两个指针 p_addr表示父进程中指向共享内存区域的指针， c_addr表示子进程中指向共享内存区域的指针
    pid_t pid;   //进程号
 
    if (argc != 2) { //如果传入参数不合法就输出样式
         fprintf(stderr, "Usage:./a.out fib_size\n\a"); 
         exit(1);
    }
    if (atof(argv[1])> 10 || atof(argv[1]) < 1) {   //判断数组大小合法性
        fprintf(stderr, "use number between 1~10\n\a"); 
        exit(1);
    }
 
 
 
    // step2------------------------------------------
 
    shared_data store;   //建立一个struct  用来暂时存储一个shared_data
 
    store.sequence_size = atof(argv[1]);    //把命令行参数传给shared_data
 
    // step3------------------------------------------
 
    if( (shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1 )   { //分配共享内存区
        fprintf(stderr, "Create Share Memory Error:%s\n\a", strerror(errno));
        exit(1);
    }
 
    p_addr = shmat(shmid, 0, 0);  
        //连接共享内存区      
    memset(p_addr, '\0', sizeof(shared_data));  
        //把数据先清零
 
    for(int i = 0; i < MAX_SEQUENCE; i++) {
        p_addr->fib_sequence[i] = store.fib_sequence[i];
    }   
 
    p_addr->sequence_size = store.sequence_size;
 
    //把数值赋给共享内存中的数值
 
    pid = fork();  //创建子进程
 
    if(pid > 0) {    //如果是父进程
 
        //在共享内存区存入一个类型为shared_data的数据store  此数据已经存储了 命令行参数
 
        wait(NULL);   
        //等待子进程结束，结束算完fib数列
 
        for (int i = 0; i < p_addr->sequence_size; i++) {
            printf("%ld ", (p_addr->fib_sequence[i]));  
        }
        //打印结果
 
        exit(0);
    }
    else if (pid == 0){      //如果是子进程， 则计算斐波那契数列 更新共享内存的数据
        c_addr = shmat(shmid, 0, 0);
        //连接内存共享区
 
        //下面计算斐波那契数列
        c_addr->fib_sequence[0] = 1;
        c_addr->fib_sequence[1] = 1;
        //直接开始就是设置0和1的位置是1 然后后面的开始算就好，因为打印是根据size来打印的，而且第一位和第二位都是1
        for (int i = 2; i < c_addr->sequence_size; i++) {
            c_addr->fib_sequence[i] = c_addr->fib_sequence[i-1] + c_addr->fib_sequence[i-2];
        }
 
 
        exit(0);
        //退出子进程
    }
 
    return 0;
}
