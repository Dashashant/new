#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 4
#define MAX 100000000

long int mas[MAX];

struct Task
{
  int index;
  int a, b;
};

long int res[N]; 

void* summary(void* task)
{
 int i, sum = 0; 

 for(i = ((struct Task*)task)->a; i < ((struct Task*)task)->b; i++)
 { 
   sum += mas[i];
 }

 res[((struct Task*)task)->index]  = sum;
 return;
}
int main()
{
    pthread_t thread_id , thread_id1, thread_id2, thread_id3, my_thread_id;
    int result, result1, result2, result3;
    int i;

    for(i = 0; i < MAX; i++)
      mas[i] = 1;
    struct Task tasks[N];
    tasks[0].index = 0;
    tasks[0].a = 0;
    tasks[0].b = MAX / N;

    tasks[1].index = 1;
    tasks[1].a = MAX / N + 1;
    tasks[1].b = 2 * MAX / N;

    tasks[2].index = 2;
    tasks[2].a = 2* MAX / N + 1;
    tasks[2].b = 3 * MAX / N;

    tasks[3].index = 3;
    tasks[3].a = 3 * MAX / N + 1;
    tasks[3].b = MAX;

    result = pthread_create(&thread_id ,
                            (pthread_attr_t *)NULL ,
                            summary ,
                            &tasks[0]);
    result1 = pthread_create(&thread_id1 ,
                            (pthread_attr_t *)NULL ,
                            summary ,
                            &tasks[1]);
    result2 = pthread_create(&thread_id ,
                            (pthread_attr_t *)NULL ,
                            summary, &tasks[2]);
    result3 = pthread_create(&thread_id1 ,
                            (pthread_attr_t *)NULL ,
                            summary, &tasks[3]);

    if (result) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
    }
    if (result1) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
    }
    if (result2) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
    }
    if (result3) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
    }

    pthread_join(thread_id , (void **) NULL);
    pthread_join(thread_id1 , (void **) NULL);
    printf("%ld", res[0] + res[1] + res[2] + res[3]);

    return 0;
}
