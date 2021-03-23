#include <stdio.h>

int main(){
	printf("fork1 is:%d\n", fork());
	printf("fork2 is:%d\n", fork());
	printf("fork3 is:%d\n", fork());
}
