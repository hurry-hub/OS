#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>

#define MAX_LEN 80      //每行命令的最大长度
#define BUFFERSIZE 11   //单个命令的最大值

typedef struct{
    int num;
    char cmd[MAX_LEN];
}command;

command buffer[BUFFERSIZE];
int buffer_pointer;
int setup(char input[], char *args[], int *background);

void print()                //打印命令
{
    int i;
    fflush(stdout);     //强迫把stdout内容输出并清空stdout
    write(STDOUT_FILENO, "\n", 1);
    for (i = (buffer_pointer - 1) % BUFFERSIZE; i != (buffer_pointer % BUFFERSIZE) && buffer[i].num; i = (i - 1 + BUFFERSIZE) % BUFFERSIZE) {
        printf("%d\t%s", buffer[i].num, buffer[i].cmd);
        fflush(stdout);
    }
}

void handle_SIGINT()          //处理中断
{
    print();
    signal(SIGINT, SIG_IGN);
    return;
}

int setup(char input[], char *argv[], int *background)
{
    int len, i, start, ct;
    //len表示命令的长度，i代表命令的个数，start代表命令的开始顺序，ct代表下一指令的位置
    pid_t pid;
    ct = 0;
    len = read(STDIN_FILENO, input, MAX_LEN);
    start = -1;
    if (len == 0) {     //未输入
        exit(0);
    }
    if (len < 0) {      //输入出错
        args[0] = NULL;
        return 0;
    }
    if (input[0] == 'r' && (input[1] == ' ' || input[1] == '\n')) {
        if (input[1] == '\n') {
            write(STDOUT_FILENO, buffer[(buffer_pointer - 1) % BUFFERSIZE].cmd, strlen(buffer[(buffer_pointer - 1) % BUFFERSIZE].cmd));
            strcpy(input, buffer[(buffer_pointer - 1) % BUFFERSIZE].cmd);
            len = strlen(input);         
        } else {
            int j;
            if (!buffer_pointer) {
                return 0;
            }
            for (j = (buffer_pointer - 1) % BUFFERSIZE; j != (buffer_pointer % BUFFERSIZE) && (buffer[j].cmd[0]  != input[2]) && buffer[j].num; j = (j - 1 + BUFFERSIZE) % BUFFERSIZE);
            if (j == (buffer_pointer % BUFFERSIZE) || buffer[j].num == 0) {
                char NF[15] = "Not Found\n";
                write(STDOUT_FILENO, NF, strlen(NF));
                return 0;
            } else {
                write(STDOUT_FILENO, buffer[j].com, strlen(buffer[j].cmd));
                strcpy(input, buffer[j].cmd);
                len = strlen(input);
            }
        }
    }
    if (input[0] != '\n') {
        buffer[(buffer_pointer % BUFFERSIZE)].num = buffer_pointer + 1;
        strcpy(buffer[(buffer_pointer % BUFFERSIZE)].cmd, input);
        buffer_pointer++;
    }
    for (i = 0; i < len; i++) {
        switch (input[i])
        {
        case ' ':
        case '\t':  
            if (start != -1) {
                args[ct] = &input[start];
                ct++;
            }
            input[i] = '\0';
            start = -1;    
            break;
        case '\n':
            if (start != -1) {
                args[ct] = &input[start];
                ct++;
            }
            input[i] = '\0';
            args[ct] = NULL;
            break;
        default:
            if (start == -1) {
                start = i;
            }
            if (input[i] == '&') {
                *background = i;
                input[i] = '\0';
            }
        }
    }
    agrs[ct] = NULL;
    return 1;
}

int main(void) {
    buffer_pointer = 0;
    memset(buffer, 0, sizeof(buffer));
    char input[MAX_LEN];
    int background;
    char *args[MAX_LEN/2 + 1];
    pid_t pid;
    char cmd[20] = "COMMAND ->";

    while (1) {
        background = 0;
        memset(input, 0, MAX_LEN);
        write(STDOUT_FILENO, cmd, strlen(cmd));
        setup(input, args, &background);
        struct sigaction handler;
        handler.sa_handler = handle_SIGINT;
        sigaction(SIGINT, &handler, NULL);

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork error!");
            exit(-1);
        } else if (pid == 0) {
            execvp(args[0], args);
            exit(0);
        } else if (!background) {
            wait(NULL);
        }
    }
    return 0;
}