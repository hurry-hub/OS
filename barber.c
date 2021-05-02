#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
int n;					//n chairs
int num;				//num customers
int flag = 1;			

void *customer(void *arg)
{
	while (flag) {
		pthread_mutex_lock(&mutex);
		if (num < n) {
			num++;
			printf("A customer comes in, there is %d chairs left.\n", n - num);
			pthread_mutex_unlock(&mutex);
		} else {
			pthread_mutex_unlock(&mutex);
			printf("There is no chair left, a customer leaves.\n");
		}
	}
}

void *barber(void *arg)
{
	while (flag) {
		if (num > 0) {
			pthread_mutex_lock(&mutex);
			num--;
			printf("A customer leaves, there is %d chairs left.\n", n - num);
			pthread_mutex_unlock(&mutex);
		}
	}
}

int main(void) 
{
	printf("Please input the sum of chairs n:");
	scanf("%d", &n);
	num = 0;
	pthread_t Barber, Customer;
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&Barber, NULL, barber, NULL);
	pthread_create(&Customer, NULL, customer, NULL);
	sleep(1);
	flag = 0;
	pthread_join(Barber, NULL);
	pthread_join(Customer, NULL);
	pthread_mutex_destroy(&mutex);
}
