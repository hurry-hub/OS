#include <iostream>
#include <pthread.h>
#include <stdio.h>

#define SIZE                10
#define NUMBER_OF_THREADS    3

void* sorter(void* params);    /* thread that performs basic sorting algorithm */
void* merger(void* params);    /* thread that performs merging of results */

int list[SIZE]={7, 12, 19, 3, 18, 4, 2, 6, 15, 8};

int result[SIZE];

typedef struct
{
    int from_index;
    int to_index;
} parameters;

int main(int argc, const char* argv[])
{
    parameters* data_1 = (parameters*)malloc(sizeof(parameters));
    parameters* data_2 = (parameters*)malloc(sizeof(parameters));
    
    pthread_t tid_sort1;
    pthread_t tid_sort2;
    pthread_t tid_merge;
    
    data_1->from_index = 0;
    data_1->to_index = 4;
    pthread_create(&tid_sort1, NULL, sorter, (void*)data_1);
    
    data_2->from_index = 5;
    data_2->to_index = 9;
    pthread_create(&tid_sort2, NULL, sorter, (void*)data_2);
    
    pthread_join(tid_sort1, NULL);
    pthread_join(tid_sort2, NULL);
    
    pthread_create(&tid_merge, NULL, merger, NULL);
    pthread_join(tid_merge, NULL);
    
    printf("The sorted array is :\n");
    for (int p = 0; p < 10; p++)
        printf("%d ", list[p]);
    return 0;
}

void* sorter(void* params){
    parameters* index;
    index=(parameters*)params;
    int temp;
    //printf("%d\t%d\n",index->from_index,index->to_index);
    for (int i = index->from_index; i <= index->to_index; i++){
        for (int j = i + 1; j <= index->to_index; j++){
            if (list[i] > list[j]){
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
        //printf("%d",list[i]);
    }
    //printf("\n");
    pthread_exit(0);
}


void* merger(void* params){
    int start = 0, end = 9, mid = 4;
    int i = start;
    int j = mid + 1;
    int temp[end + 1];
    int k = 0;
    while (i <= mid && j <= end) {
        if (list[i] < list[j]){
            temp[k++] = list[i++];
        }
        else{
            temp[k++] = list[j++];
        }
    }
    if (i == mid + 1) {
        while(j <= end)
            temp[k++] = list[j++];
    }
    if (j == end + 1) {
        while (i <= mid)
            temp[k++] = list[i++];
    }
    for (j = 0, i = start ; j < k; i++, j++) {
        list[i] = temp[j];
    }
    pthread_exit(0);
}

