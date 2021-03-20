#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#define _GNU_SOURCE

int main(){
	syscall(335);
	return 0;
}
