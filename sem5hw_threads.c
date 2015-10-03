#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 4
#define MAX 100000000

long int mas[MAX];

struct Thread
{
  pthread_t id;
  int result;
};
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
    struct Thread threads[N];
    
    for(i = 0; i < N; i++)
    {
      tasks[i].index = 0;
      tasks[i].a = i * MAX / N ;
      tasks[i].b = (i + 1) * MAX / N;
    }

    for(i = 0; i < N; i++)
    {
      threads[i].result = pthread_create(&(threads[i].id) ,
                            (pthread_attr_t *)NULL ,
                            summary ,
                            &tasks[i]);
      
      if (threads[i].result) {
        printf("Can`t create thread, returned value = %d\n" , result);
        exit(-1);
      }
    }

    for(i = 0; i < N; i++)
    {
      pthread_join(threads[i].id , (void **) NULL);
    }
    printf("%ld", res[0] + res[1] + res[2] + res[3]);

    return 0;
}
