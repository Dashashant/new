#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4
#define MAX 100000000
long int * array;

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
    sum += array[i];
  }

  res[((struct Task*)task)->index] = sum;
  return;
}

int main()
{
  int i;
  array = (long int *)malloc(MAX * sizeof(long int));

  for(i = 0; i < MAX; i++)
    array[i] = rand() % 2;

  struct Task tasks[N];
  struct Thread threads[N];

  for(i = 0; i < N; i++)
  {
    tasks[i].index = 0;
    tasks[i].a = i * MAX / N ;
    tasks[i].b = (i + 1) * MAX / N;
  }
  if (MAX % N != 0);
    tasks[N-1].b = MAX;

  for(i = 0; i < N; i++)
  {
    threads[i].result = pthread_create(&(threads[i].id) ,
                             (pthread_attr_t *)NULL ,
                             summary ,
                             &tasks[i]);

    if (threads[i].result) {
       printf("Can`t create thread, returned value = %d\n" ,
       threads[i].result);
       exit(-1);
    }
  }

  for(i = 0; i < N; i++)
  {
    pthread_join(threads[i].id , (void **) NULL);
  }

  long int result = 0;
  long int D = 0;

  for(i = 0; i < N; i++)
  {
    result += res[i];
  }

  for(i = 0; i < N; i++)
  {
    D += (array[i] - result/N) * (array[i] - result/N);
  }

  printf("average %ld, dispertion %ld  ", result/N, D/N);
  return 0;
}
